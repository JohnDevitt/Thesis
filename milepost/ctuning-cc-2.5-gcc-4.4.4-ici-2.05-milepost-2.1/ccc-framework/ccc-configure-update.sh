#!/bin/bash

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

FILE_CCC_BLD="ccc-build.cfg"
FILE_CCC_TMP="tmp-ccc.tmp"

################################################################################
echo "*************** CCC: Check for updates ***************"

upd=0
echo ""
echo "Would you like to try to check for updates (Y/N)?"
 
read var1
   
if [ "$var1" != "Y" ] && [ "$var1" != "y" ] ; then
 rm -f *.tmp
 return
 exit
fi

ccc_build=""
if [ ! -z "$FILE_CCC_BLD" ] ; then
 exec 9< $FILE_CCC_BLD
 read <&9 ccc_build
fi

rm -f latest_build.txt
wget "http://cTuning.org/software/ccc/latest_build.txt"

if [ ! -f "latest_build.txt" ] ; then
 echo "Can't download latest CCC build number ..."
 return
 exit
fi

exec 9< latest_build.txt
read <&9 ccc_build_latest

rm -f latest_build.txt

echo "CCC current build number: " $ccc_build
echo "CCC  latest build number: " $ccc_build_latest

echo ""
if [ $ccc_build -lt $ccc_build_latest ] ; then
  echo "You CCC version is outdated."
  echo "You can find more information about CCC updates at http://cTuning.org"
  return
  exit
fi

echo "You version is up to date!"


