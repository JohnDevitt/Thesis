#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2004-2009, UNIDAPT Group, INRIA, France

################################################################################
echo "*************** CCC: Configure platform name ***************"

export PATH=$PWD/src-plat-indep/plugins:$PATH

### Check/select platform
if [ ! -z "$CCC_PLATFORM" ] ; then
 echo "Current CCC platform is $CCC_PLATFORM."
 echo ""
 echo "Would you like to configure a new platform (Y/N)?"
 
 read var1
   
 if [ "$var1" = "Y" ] || [ "$var1" = "y" ] ; then
  export CCC_PLATFORM=
 else
  exit
 fi  
fi

if [ -z "$CCC_PLATFORM" ] ; then
 echo ""
 echo "Starting new platform configuration ..."

 echo ""
 echo "Enter local platform extension (for example x86_gcc_430, ia64_open64_41, etc):"
 read CCC_PLATFORM
 export CCC_PLATFORM

 ### Check if cfg directory exists
 upd=0
 if [ -d "cfg/$CCC_PLATFORM" ] ; then
  echo ""
  echo "Directory for CCC configuration files (cfg/$CCC_PLATFORM) already exists! Remove it (Y/N)?"
 
  read var1
   
  if [ "$var1" = "Y" ] || [ "$var1" = "y" ] ; then
   echo ""
   echo "Deleting cfg/$CCC_PLATFORM ..."

   rm -rf cfg/$CCC_PLATFORM
         
   upd=0
  else
   upd=1
  fi
 fi

 if [ $upd -eq "0" ] ; then
  echo ""

  echo "Creating configuration directory cfg/$CCC_PLATFORM directory ..."
  mkdir cfg/$CCC_PLATFORM

  echo "Copying default configuration files to the cfg/$CCC_PLATFORM directory ..."
  cp -rf cfg/_default/README.txt cfg/$CCC_PLATFORM
 fi

 ### Check if install directory exists
 upd=0
 if [ -d "install/$CCC_PLATFORM" ] ; then
  echo ""
  echo "Directory for CCC binary installation (install/$CCC_PLATFORM) already exists! Remove it (Y/N)?"
 
  read var1
   
  if [ "$var1" = "Y" ] || [ "$var1" = "y" ] ; then
   echo ""
   echo "Deleting install/$CCC_PLATFORM ..."

   rm -rf install/$CCC_PLATFORM
  fi
 fi

fi

rm -f *.tmp
