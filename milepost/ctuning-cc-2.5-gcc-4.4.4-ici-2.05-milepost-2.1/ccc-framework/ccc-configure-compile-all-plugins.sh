#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2004-2009, UNIDAPT Group, INRIA, France

FILE_CCC_TMP="tmp-ccc.tmp"

#go to all dirs recursively
GLB_VAR=0;
GLB_DEST="";
addinclude="";

function recursive
{
 cd "$1" 2> /dev/null
 GLB_VAR=$?
 if [ $GLB_VAR -eq 0 ]
 then

  if [ -f "Makefile" ] ; then
    echo "============================================================="
    echo "Compiling $PWD ..."
    echo

    if [ $db = "0" ] && [ -f "_ccc_use_db" ] ; then
     echo "  Since database is currently not used, ignore this program ..."
    else
     make clean

     make MISCOPT=-O3 EXTRAINCLUDE=$addinclude
     if [ "${?}" != "0" ] ; then
      echo ""
      echo "CCC Error: Compilation failed!" 
      echo "Check $PWD for problems ..."
      rm -f *.tmp
      exit
      return
     fi
     make install DEST=$GLB_DEST
     make clean
    fi
  fi 

  for i in *
  do
   if [ "$i" = "*" -o -h "$i" -o -c "$i" ] ; then
    continue
   fi
   if [ -d "$i" -a -r "$i" ] ; then
    recursive "$i"
    if [ $GLB_VAR -eq 0 ] ; then
     cd ..
    else
     GLB_VAR=0;
    fi
   else
    #$i - filename

    continue
   fi
  done
 fi
}

################################################################################
echo "*************** CCC: Compile all plugins ***************"

export PATH=$PWD/src-plat-indep/plugins:$PATH

### Check/select platform
if [ -z "$CCC_PLATFORM" ] ; then
 echo ""
 echo "CCC Platform has not been yet configured since \$CCC_PLATFORM variable is empty."
 echo ""
 echo "Run INSTALL.sh first!"
 rm -f *.tmp
 exit
fi

if [ ! -d "install" ] ; then
 mkdir install
fi

### Check if database is used
db=1
if [ -z "$CCC_URL" ] ; then
 db=0
 echo ""
 echo "Collective Optimization Database is not used."
 echo "Programs that do not use COD will not be compiled..."
fi

### Check if directory exists
if [ -d "install/$CCC_PLATFORM" ] ; then
 if [ -z "$CURINSTALL" ] ; then
  echo ""
  echo "Directory install/$CCC_PLATFORM exists! Remove (Y/N)?"

  read var1

  if [ "$var1" = "Y" ] || [ "$var1" = "y" ] ; then
   echo ""
   echo "Deleting install/$CCC_PLATFORM ..."

   rm -rf install/$CCC_PLATFORM

   mkdir install/$CCC_PLATFORM 
  fi
 fi
else
 echo ""
 echo "Create install/$CCC_PLATFORM directory ..."

 mkdir install/$CCC_PLATFORM 
fi

GLB_DEST=$PWD/install/$CCC_PLATFORM
GLB_CFG=$PWD/cfg

CUR=$PWD

echo ""
echo "Enter extra include directories including -I such as -I/usr/local/include"
echo "  for example if your MySQL header files are not in the default include directory."
echo "  Press <Enter> if you want to skip it..."
echo ""

read addinclude

recursive $CUR/src-plat-dep/plugins

echo "============================================================="
echo "Compilation finished!"
