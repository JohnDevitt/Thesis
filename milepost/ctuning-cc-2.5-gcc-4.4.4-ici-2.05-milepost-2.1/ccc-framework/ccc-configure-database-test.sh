#!/bin/bash

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

FILE_CCC_BLD_DB="ccc-build-db.cfg"
FILE_CCC_BLD="ccc-build.cfg"
FILE_CCC_TMP="tmp-ccc.tmp"
FILE_CCC_TMP1="tmp-ccc1.tmp"
FILE_CCC_TMP2="tmp-ccc2.tmp"

################################################################################
echo "*************** CCC: Test databases ***************"

export PATH=$PWD/src-plat-indep/plugins:$PATH

### Check if files with versions exist
echo ""

ccc_build=""
if [ ! -z "$FILE_CCC_BLD" ] ; then
 exec 9< $FILE_CCC_BLD
 read <&9 ccc_build

 echo "CCC Build:                                                    " $ccc_build
fi

ccc_build_db1=""
ccc_build_db2=""
if [ ! -z "$FILE_CCC_BLD_DB" ] ; then
 exec 9< $FILE_CCC_BLD_DB
 read <&9 ccc_build_db1
 read <&9 ccc_build_db2

 echo "This version can work with the range of versions of databases:" $ccc_build_db1 "..." $ccc_build_db2
fi

### Check/select platform
if [ -z "$CCC_PLATFORM" ] ; then
 echo ""
 echo "CCC Platform has not been yet configured since \$CCC_PLATFORM variable is empty."
 echo ""
 echo "Run INSTALL.sh first!"
 rm -f *.tmp
 exit
fi

# Read database info
upd=0
if [ ! -z "$CCC_URL" ] ; then
 . ccc--db-cur-info.sh
else
 echo ""
 echo "CCC is currently configured not to use MySQL database. Skipping this test ..."
 rm -f *.tmp
 return
 exit
fi

  #Check mysql
  echo "============================================================="
  echo ""
  echo "Checking \"mysql\" program ..."
  mysql --help > $FILE_CCC_TMP 2> $FILE_CCC_TMP

  i=0
  exec 9< $FILE_CCC_TMP
  while <&9 read line ;
  do
   let i=i+1
  done

  rm -f $FILE_CCC_TMP

  if [ $i -lt "2" ] ; then
   echo ""
   echo "\"mysql\" program doesn't exist ... You have to install it ..."
   echo "Would you like to STOP using database for your experiments (Y/N)?"

   read var1
   
   if [ "$var1" = "Y" ] || [ "$var1" = "y" ] ; then
    export CCC_URL=
    rm -f *.tmp
    return
    exit
   fi
  else
   echo ""
   echo "MySQL client exists, continue ..."
  fi

#Test Common database
echo ""
echo "Testing version of COMMON database ..."

if [ ! -z "$CCC_C_URL" ] ; then
 echo "select VERSION from INFORMATION" > $FILE_CCC_TMP
 mysql -h $CCC_C_URL -u $CCC_C_USER --password=$CCC_C_PASS $CCC_C_SSL $CCC_C_DB < $FILE_CCC_TMP > $FILE_CCC_TMP1 2> $FILE_CCC_TMP2
 linelast=""

 echo ""
 echo "Error response from server (if any):"
 exec 8< $FILE_CCC_TMP2
 while <&8 read line ;
 do
  echo $line
 done

 i=0
 echo ""
 echo "Response from server:"
 exec 9< $FILE_CCC_TMP1
 while <&9 read line ;
 do
  linelast=$line
  echo $line
  let i=i+1
 done

 echo ""
 if [ $i -lt 2 ] ; then
  echo "Error: can't find version of the database. Maybe incompatible versions. Exiting ..."
  rm -f *.tmp
  return
  exit
 fi

 echo "Database version:" $linelast
 
 if [ $linelast -lt $ccc_build_db1 ] || [ $linelast -gt $ccc_build_db2 ] ; then
  echo "  Database version is not supported. You should update CCC Framework."
  rm -f *.tmp
  return
  exit
 fi

 echo "  Database version is ok."
else
 echo "  IP is empty, hence this database is not used by CCC Framework ..."
fi

#Test database with experiments
echo ""
echo "Testing version of database for EXPERIMENTS ..."

if [ ! -z "$CCC_URL" ] ; then
 echo "select VERSION from INFORMATION" > $FILE_CCC_TMP
 mysql -h $CCC_URL -u $CCC_USER --password=$CCC_PASS $CCC_SSL $CCC_DB < $FILE_CCC_TMP > $FILE_CCC_TMP1 2> $FILE_CCC_TMP2

 echo ""
 echo "Error response from server (if any):"
 exec 8< $FILE_CCC_TMP2
 while <&8 read line ;
 do
  echo $line
 done

 linelast=""
 i=0
 echo ""
 echo "Response from server:"
 exec 9< $FILE_CCC_TMP1
 while <&9 read line ;
 do
  echo $line
  linelast=$line
  let i=i+1
 done
 
 echo ""
 if [ $i -lt 2 ] ; then
  echo "Error: can't find version of the database. Maybe incompatible versions. Exiting ..."
  rm -f *.tmp
  return
  exit
 fi

 echo "Database version:" $linelast
 
 if [ $linelast -lt $ccc_build_db1 ] || [ $linelast -gt $ccc_build_db2 ] ; then
  echo "  Database version is not supported. You should update CCC Framework"
  rm -f *.tmp
  return
  exit
 fi

 echo "  Database version is ok."
else
 echo "  IP is empty, hence this database is not used by CCC Framework ..."
fi

#Test collective tuning database
echo ""
echo "Testing version of Shared COLLECTIVE TUNING database"

if [ ! -z "$CCC_CT_URL" ] ; then
 echo "select VERSION from INFORMATION" > $FILE_CCC_TMP
 mysql -h $CCC_CT_URL -u $CCC_CT_USER --password=$CCC_CT_PASS $CCC_CT_SSL $CCC_CT_DB < $FILE_CCC_TMP > $FILE_CCC_TMP1 2> $FILE_CCC_TMP2

 echo ""
 echo "Error response from server (if any):"
 exec 8< $FILE_CCC_TMP2
 while <&8 read line ;
 do
  echo $line
 done

 linelast=""
 i=0
 echo ""
 echo "Response from server:"
 exec 9< $FILE_CCC_TMP1
 while <&9 read line ;
 do
  echo $line
  linelast=$line
  let i=i+1
 done

 echo ""
 if [ $i -lt 2 ] ; then
  echo "Error: can't find version of the database. Maybe incompatible versions. Exiting ..."
  rm -f *.tmp
  return
  exit
 fi

 echo "Database version:" $linelast
 
 if [ $linelast -lt $ccc_build_db1 ] || [ $linelast -gt $ccc_build_db2 ] ; then
  echo "  Database version is not supported. You should update CCC Framework"
  rm -f *.tmp
  return
  exit
 fi

 echo "  Database version is ok."
else
 echo "  IP is empty, hence this database is not used by CCC Framework ..."
fi

rm -f $FILE_CCC_TMP
rm -f $FILE_CCC_TMP1
rm -f $FILE_CCC_TMP2
