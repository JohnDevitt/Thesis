#!/bin/bash

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

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
echo "   DB name : " $CCC_CTS_DB
echo "   Username: " $CCC_CTS_USER

echo "******************************************************************"
