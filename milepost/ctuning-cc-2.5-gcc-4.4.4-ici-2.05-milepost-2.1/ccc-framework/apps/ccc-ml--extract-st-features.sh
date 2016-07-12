#!/bin/bash

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

##############################################################
#to export passes info from GCC if supported
#export CCC_ICI_PASSES_RECORD=1
#export CCC_ICI_PASSES_USE=1
#export CCC_ICI_PASSES_OPT_BASE=-O3
#export ICI_PASSES_ALL=...
export ICI_PLUGIN_VERBOSE=1
export ICI_VERBOSE=1

export CCC_ICI_FEATURES_STATIC_EXTRACT=1
export ICI_PROG_FEAT_PASS=fre

#to record run-time background info
#(when working with realistic optimization scenarios)
#export CCC_RUN_TIME_BACKGROUND="matmul 16Mb array, etc"

#export CCC_HC_PAPI_USE=$CCC_HC_PAPI_LIST
#export CCC_HC_PAPI_USE=PAPI_L1_DCMx,PAPI_L2_DCMx,PAPI_TLB_DMx,PAPI_L1_LDMx,PAPI_L1_STMx,PAPI_L2_LDMx,PAPI_L2_STMx,PAPI_BR_TKNx,PAPI_BR_MSPx,PAPI_TOT_INSx,PAPI_FP_INSx,PAPI_BR_INSx,PAPI_VEC_INSx,PAPI_TOT_CYCx,PAPI_L1_DCHx,PAPI_FP_OPSx

#export CCC_GPROF=1

#to perform only compilation
#export CCC_NO_RUN=1
##############################################################

export TIMED_RUN="timed-run 3000"

CURDIR=$PWD

. ccc--select-db.sh

exec 6< ccc--bench-list.txt
while <&6 read line ;
do 
 if [ -n "$line" ]; then
  echo Processing "${line}"-tmp ...
  cd  $CURDIR/${line}-tmp;

  echo "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
  echo $PWD
  echo "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

  ccc-comp gcc422 -O3
  ccc-run 1 1

 else
  echo ""
 fi
done
