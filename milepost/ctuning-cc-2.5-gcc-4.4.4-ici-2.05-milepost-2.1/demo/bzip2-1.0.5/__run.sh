#!/bin/bash

# Copyright (C) 2007-2010 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

source ./___common_environment.sh

read -s -p "  Enter cTuning password for $CCC_CTS_USER : " pass
export CCC_CTS_PASS=$pass
echo ""

#Select only a few functions to process when predicting optimizations
#export ICI_FILE_SELECT_FUNCTIONS=$PWD/_ctuning_select_functions.txt

time ./__run1.sh

echo ""
echo "Output:"
more ftmp_out