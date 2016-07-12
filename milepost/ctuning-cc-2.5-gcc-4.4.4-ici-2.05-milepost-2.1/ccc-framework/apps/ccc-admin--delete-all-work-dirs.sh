#!/bin/bash

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

#go to all dirs recursively
GLB_VAR=0;
GLB_DEST="";

function recursive
{
 cd "$1" 2> /dev/null
 GLB_VAR=$?
 if [ $GLB_VAR -eq 0 ]
 then
  echo $PWD
  rm -rf src-ccc-tmp
  rm -rf src-tmp

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
echo "*************** CCC: Compile all tools ***************"

export PATH=.:$PATH

echo "Removing src-ccc-tmp from all directories ..."
echo ""

recursive .

