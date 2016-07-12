#!/bin/bash

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

. ./configure_spec2006.sh

#Check if ignore
if [ "$DB_PARAM_IGNORE" != "1" ] ; then

#Load local info
if [ -f "ccc--select-db-local.sh" ] ; then
 . ./ccc--select-db-local.sh
fi

echo "******************************************************************"

echo "Checking info for COMMON database ..."
if [ "$CCC_C_URL" = "" ] ; then
 read -p "  Enter C_URL : " var
 export CCC_C_URL=$var
fi
if [ "$CCC_C_DB" = "" ] ; then
 read -p "  Enter C_DB  : " var
 export CCC_C_DB=$var
fi
if [ "$CCC_C_USER" = "" ] ; then
 read -p "  Enter C_USER: " var
 export CCC_C_USER=$var
fi
if [ "$CCC_C_PASS" = "" ] ; then
 read -p "  Enter C_PASS: " var
 export CCC_C_PASS=$var
fi

echo "Checking info for database with EXPERIMENTS ..."
if [ "$CCC_URL" = "" ] ; then
 read -p "  Enter URL : " var
 export CCC_URL=$var
fi
if [ "$CCC_DB" = "" ] ; then
 read -p "  Enter DB  : " var
 export CCC_DB=$var
fi
if [ "$CCC_USER" = "" ] ; then
 read -p "  Enter USER: " var
 export CCC_USER=$var
fi
if [ "$CCC_PASS" = "" ] ; then
 read -p "  Enter PASS: " var
 export CCC_PASS=$var
fi

echo "Checking info for COLLECTIVE TUNING database ..."
if [ "$CCC_CT_URL" = "" ] ; then
 read -p "  Enter URL : " var
 export CCC_CT_URL=$var
fi
if [ "$CCC_CT_DB" = "" ] ; then
 read -p "  Enter DB  : " var
 export CCC_CT_DB=$var
fi
if [ "$CCC_CT_USER" = "" ] ; then
 read -p "  Enter USER: " var
 export CCC_CT_USER=$var
fi
if [ "$CCC_CT_PASS" = "" ] ; then
 read -p "  Enter PASS: " var
 export CCC_CT_PASS=$var
fi

echo "******************************************************************"

echo "Information about COMMON database:"
echo "   IP      : " $CCC_C_URL
echo "   DB name : " $CCC_C_DB
echo "   Username: " $CCC_C_USER
echo "Information for the database for EXPERIMENTS:"
echo "   IP      : " $CCC_URL
echo "   DB name : " $CCC_DB
echo "   Username: " $CCC_USER
echo "Information about COLLECTIVE TUNING database:"
echo "   IP      : " $CCC_CT_URL
echo "   DB name : " $CCC_CT_DB
echo "   Username: " $CCC_CT_USER
echo "Information about COLLECTIVE TUNING services:"
echo "   URL     : " $CCC_CTS_URL
echo "   Username: " $CCC_CTS_USER

echo "******************************************************************"

fi
