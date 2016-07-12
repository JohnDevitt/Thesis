#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2004-2009, UNIDAPT Group, INRIA, France

FILE_CCC_TMP="tmp-ccc.tmp"
FILE_CCC_TMP1="tmp-ccc1.tmp"
FILE_CCC_TMP2="tmp-ccc2.tmp"
FILE_CCC_TMP3="tmp-ccc3.tmp"

################################################################################
echo "*************** CCC: Configure environment ***************"

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

echo ""
echo "Choosing existing or generating new UUID for the environment:"

if [ ! -z "$CCC_ENVIRONMENT_ID" ] ; then
 echo "Current CCC_ENVIRONMENT_ID=$CCC_ENVIRONMENT_ID"
 echo ""
 echo "Would you like to regenerate it (Y/N)?"
 read var
 
 if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
  echo ""
 else
  return
  exit
 fi
fi

echo ""
echo "Would you like to enter existing CCC_ENVIRONMENT_ID (Y/N)?"
echo "  You can find the most similar one in the Collective Optimization Database (http://cTuning.org/cdatabase)"
echo "  to profit immediately from the optimizations shared by the community,"
echo "  otherwise a new unique ID will be generated and you are warmly welcome" 
echo "  to share this data with the cTuning community too to enable collective optimization." 

read var

if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
 echo ""
 echo "Enter CCC_ENVIRONMENT_ID:"
 read CCC_ENVIRONMENT_ID
 export CCC_ENVIRONMENT_ID
 
 echo ""
 echo "If you run this script outside INSTALL.sh, you should change this variable"
 echo "in the cfg/$CCC_PLATFORM/ccc-env.sh script!"

 return
 exit 
fi

ccc-uuidgen > $FILE_CCC_TMP
exec 9< $FILE_CCC_TMP
read <&9 uuid1
read <&9 uuid2
rm -rf $FILE_CCC_TMP
CCC_ENVIRONMENT_ID=$uuid1$uuid2
export CCC_ENVIRONMENT_ID

echo ""
echo "Generating new CCC_ENVIRONMENT_ID"
echo "New CCC_ENVIRONMENT_ID=$CCC_ENVIRONMENT_ID"

###### read database info ######
if [ -z "$CCC_URL" ] ; then
 echo ""
 echo "Since \$CCC_URL = \"\", it means that you decided not to use" 
 echo "  Collective Optimization Database. Later, if you change you mind,"
 echo "  you can install and configure it using appropriate individual installation script..."
 rm -f *.tmp
 return
 exit
fi

. ccc--db-cur-info.sh

###### check if environment exists ######

echo "select * from ENVIRONMENTS where (ENVIRONMENT_ID=\"$CCC_ENVIRONMENT_ID\")" > $FILE_CCC_TMP
mysql -h $CCC_C_URL -u $CCC_C_USER --password=$CCC_C_PASS $CCC_C_SSL $CCC_C_DB < $FILE_CCC_TMP > $FILE_CCC_TMP1

i=0
exec 9< $FILE_CCC_TMP1
while <&9 read line ;
do
 let i=i+1
done

if [ $i -ne 0 ] ; then
 echo ""
 echo "Record already exists in the database with this CCC_ENVIRONMENT_ID:"
 echo ""
 cat $FILE_CCC_TMP1

 rm -f *.tmp
 return
 exit
fi

### Show some info about environment
echo ""
echo "Would you like to view some local info about your environment (Y/N)?"
read var
 
if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
 echo "*************** Some info about current environment (uname): ***************"
 uname -a

 echo ""
 echo "*************** Enter the following information for this environment: ***************"
fi

###### check fileds ######

echo "show FIELDS from ENVIRONMENTS" > $FILE_CCC_TMP
mysql -h $CCC_C_URL -u $CCC_C_USER --password=$CCC_C_PASS $CCC_C_SSL $CCC_C_DB < $FILE_CCC_TMP > $FILE_CCC_TMP1
rm -rf $FILE_CCC_TMP

echo ""
echo "Available fields:"
echo ""

i=0
fin=0
exec 9< $FILE_CCC_TMP1
while <&9 read line ;
do
 if [ $i -gt "0" ] && [ $fin -ne "1" ]; then
   fld=${line%%	*}
   if [ $fld = "DATETIME" ] ; then
    fin=1
   else
    echo "$fld:"
   fi
 fi
 let i=i+1
done

echo ""

echo -n "insert into ENVIRONMENTS (ENVIRONMENT_ID" > $FILE_CCC_TMP
echo -n "values (\"$CCC_ENVIRONMENT_ID\"" > $FILE_CCC_TMP2

i=0
fin=0
exec 9< $FILE_CCC_TMP1
while <&9 read line ;
do
 if [ $i -gt "1" ] && [ $fin -ne "1" ] ; then
   fld=${line%%	*}

   if [ $fld = "DATETIME" ] ; then
    fin=1
   else
    echo "$fld:"
    read xfld
   
    echo -n ,$fld >> $FILE_CCC_TMP
    echo -n ,\"$xfld\" >> $FILE_CCC_TMP2
   fi
 fi
 let i=i+1
done

 #Add datetime
  fld="DATETIME"
   date '+%F %T' > $FILE_CCC_TMP3
   exec 8< $FILE_CCC_TMP3
   read <&8 xfld
   rm -rf $FILE_CCC_TMP3
  echo -n ,$fld >> $FILE_CCC_TMP
  echo -n ,\"$xfld\" >> $FILE_CCC_TMP2
   
 #Add IP   
  fld="IP"
   ccc-read-ip > $FILE_CCC_TMP3
   exec 8< $FILE_CCC_TMP3
   read <&8 xfld
   rm -rf $FILE_CCC_TMP3
  echo -n ,$fld >> $FILE_CCC_TMP
  echo -n ,\"$xfld\" >> $FILE_CCC_TMP2

 #Add USER
  fld="USER"
  xfld=$CCC_CTS_USER
  echo -n ,$fld >> $FILE_CCC_TMP
  echo -n ,\"$xfld\" >> $FILE_CCC_TMP2

echo -n ") " >> $FILE_CCC_TMP
echo -n ")" >> $FILE_CCC_TMP2

cat $FILE_CCC_TMP2 >> $FILE_CCC_TMP

echo ""
echo "Recording data to the database ..."

mysql -h $CCC_C_URL -u $CCC_C_USER --password=$CCC_C_PASS $CCC_C_SSL $CCC_C_DB < $FILE_CCC_TMP

rm -f *.tmp
