#!/bin/bash

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

FILE_CCC_TMP="tmp-ccc.tmp"

################################################################################
echo "*************** CCC: Configure database ***************"

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

# Check database info
upd=0
if [ ! -z "$CCC_URL" ] ; then
 . ccc--db-cur-info.sh
 
 echo ""
 echo "Would you like to provide new information or even stop using Collective Optimization Database (Y/N)?"
 read var
 
 if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
  upd=1
 fi
else
 upd=1
fi

if [ $upd -eq "1" ] ; then
 echo ""
 echo "Would you like to use Collective Optimization Database for your experiments (Y/N)?"
 echo "  Using COD (http://cTuning.org/cdatabase) is a preferred method which allows you"
 echo "  to profit from the optimization cases collected by the cTuning community"
 echo "  (to improve execution time, code size, compilation time"
 echo "  and other important characteristics) and also to easily share you data with the community"
 echo "  to enable collective optimization and machine learning. If you decide to use COD,"
 echo "  you should have MySQL client installed on your machine."
 echo "  If you decide not to use COD, your compilation, optimization and execution statistics"
 echo "  will be saved in local text files, which you can later add to COD using CCC tools)"
 read var
 
 if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then

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

  echo ""
  echo "You can find more information about CCC database at http://cTuning.org/cdatabase"
  echo "You can also obtain login info for databases there."

  #Common database
  echo ""
  echo "Information about COMMON database:"
  echo ""
 
  echo "Provide IP address of the COMMON database (<Enter> for cTuning.org):"
  read CCC_C_URL
  if [ -z $CCC_C_URL ] ; then
   CCC_C_URL="cTuning.org"
  fi
  export CCC_C_URL
  
  echo "Provide database name (<Enter> for cod_common):"
  read CCC_C_DB
  if [ -z $CCC_C_DB ] ; then
   CCC_C_DB="cod_common"
  fi
  export CCC_C_DB

  echo "Provide database username (<Enter> for ctuserc):"
  read CCC_C_USER
  if [ -z $CCC_C_USER ] ; then
   CCC_C_USER="ctuserc"
  fi
  export CCC_C_USER

  echo "Provide database password (<Enter> for fsdkl328zAsj):"
  read CCC_C_PASS
  if [ -z $CCC_C_PASS ] ; then
   CCC_C_PASS="fsdkl328zAsj"
  fi
  export CCC_C_PASS

  echo "Provide mysql option for SSL, if used (<Enter for '', example '--ssl-ca /dev/null'):"
  read CCC_C_SSL
  if [ -z $CCC_C_SSL ] ; then
#   CCC_C_SSL="--ssl-ca /dev/null"
   CCC_C_SSL=""
  fi
  export CCC_C_SSL

  #Database for experiments
  echo ""
  echo "Information about database for experiments:"
  echo ""
 
  echo "Provide IP address of the database for EXPERIMENTS (<Enter> for cTuning.org):"
  read CCC_URL
  if [ -z $CCC_URL ] ; then
   CCC_URL="cTuning.org"
  fi
  export CCC_URL

  echo "Provide database name (<Enter> for cod_opt_cases_test):"
  read CCC_DB
  if [ -z $CCC_DB ] ; then
   CCC_DB="cod_opt_cases_test"
  fi
  export CCC_DB

  echo "Provide database username (<Enter> for ctuser2):"
  read CCC_USER
  if [ -z $CCC_USER ] ; then
   CCC_USER="ctuser2"
  fi
  export CCC_USER

  echo "Provide database password (<Enter> for kvire923klzx):"
  read CCC_PASS
  if [ -z $CCC_PASS ] ; then
   CCC_PASS="kvire923klzx"
  fi
  export CCC_PASS

  echo "Provide mysql option for SSL, if used (<Enter for '', example '--ssl-ca /dev/null'):"
  read CCC_SSL
  if [ -z $CCC_SSL ] ; then
#   CCC_SSL="--ssl-ca /dev/null"
   CCC_SSL=""
  fi
  export CCC_SSL

  #Collective Tuning Database
  echo ""
  echo "Information about Shared Collective Optimization Database:"
  echo "(if you want to share your interesting optimization cases with the community)"
  echo "You can find more info at http://cTuning.org/cdatabase"
  echo ""
 
  echo "Provide IP address of the Collective Tuning database (<Enter> for cTuning.org):"
  read CCC_CT_URL
  if [ -z $CCC_CT_URL ] ; then
   CCC_CT_URL="cTuning.org"
  fi
  export CCC_CT_URL

  echo "Provide database name (<Enter> for cod_opt_cases_all):"
  read CCC_CT_DB
  if [ -z $CCC_CT_DB ] ; then
   CCC_CT_DB="cod_opt_cases_all"
  fi
  export CCC_CT_DB

  echo "Provide database username (<Enter> for ctuser1):"
  read CCC_CT_USER
  if [ -z $CCC_CT_USER ] ; then
   CCC_CT_USER="ctuser1"
  fi
  export CCC_CT_USER

  echo "Provide database password (<Enter> for gkx49xlks7Fa):"
  read CCC_CT_PASS
  if [ -z $CCC_CT_PASS ] ; then
   CCC_CT_PASS="gkx49xlks7Fa"
  fi
  export CCC_CT_PASS

  echo "Provide mysql option for SSL, if used (<Enter for '', example '--ssl-ca /dev/null'):"
  read CCC_CT_SSL
  if [ -z $CCC_CT_SSL ] ; then
#   CCC_CT_SSL="--ssl-ca /dev/null"
   CCC_CT_SSL=""
  fi
  export CCC_CT_SSL

  #Collective Tuning services
  echo ""
  echo "Information about Collective Tuning web-services (if you are registered at cTuning.org):"
  echo ""
 
  echo "Provide URL of the Collective Tuning services (<Enter> for cTuning.org/wiki/index.php/Special:CDatabase?request=):"
  read CCC_CTS_URL
  if [ -z $CCC_CTS_URL ] ; then
   CCC_CTS_URL="cTuning.org/wiki/index.php/Special:CDatabase?request="
  fi
  export CCC_CTS_URL

  echo "Provide database name (<Enter> for cod_opt_cases_all):"
  read CCC_CTS_DB
  if [ -z $CCC_CTS_DB ] ; then
   CCC_CTS_DB="cod_opt_cases_all"
  fi
  export CCC_CT_DB

  echo "Provide cTuning.org username:"
  read CCC_CTS_USER
  export CCC_CTS_USER

  echo "Provide cTuning.org password:"
  read CCC_CTS_PASS
  export CCC_CTS_PASS

 else
  export CCC_URL=

  echo ""
  echo "Environment variable CCC_URL has been set to \"\""
  echo "which indicates that you do not use MySQL database for your experiments ..."
  echo ""
  echo "If you run this script outside INSTALL.sh, you should change this variable"
  echo "in the cfg/$CCC_PLATFORM/ccc-env.sh script!"
  rm -f *.tmp
 fi
 
fi
