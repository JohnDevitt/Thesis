#!/bin/bash

# Copyright (C) 2004-2009 by Grigori G.Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

##############################################################
#Record compiler passes (through ICI)
#export CCC_ICI_PASSES_RECORD=1

#Load compiler passes from files or environment (through ICI)
#export CCC_ICI_PASSES_USE=1
#export CCC_ICI_PASSES_OPT_BASE=-O3
#export ICI_PASSES_ALL=...

#Produce verbose output from the ICI plugins
#export ICI_PLUGIN_VERBOSE=1
#export ICI_VERBOSE=1

#Extract program static features (through ICI)
#export CCC_ICI_FEATURES_STATIC_EXTRACT=1
#export ICI_PROG_FEAT_PASS=fre

#Record run-time background info when working in realistic environments 
#to know how other applications interfere with optimizations
#export CCC_RUN_TIME_BACKGROUND="matmul 16Mb array, etc"

#Profile application using hardware counters and PAPI library
#export CCC_HC_PAPI_USE=$CCC_HC_PAPI_LIST
#export CCC_HC_PAPI_USE=PAPI_L1_DCMx,PAPI_L2_DCMx,PAPI_TLB_DMx,PAPI_L1_LDMx,PAPI_L1_STMx,PAPI_L2_LDMx,PAPI_L2_STMx,PAPI_BR_TKNx,PAPI_BR_MSPx,PAPI_TOT_INSx,PAPI_FP_INSx,PAPI_BR_INSx,PAPI_VEC_INSx,PAPI_TOT_CYCx,PAPI_L1_DCHx,PAPI_FP_OPSx

#Profile application using gprof
#export CCC_GPROF=1

#Profile application using oprof
#export CCC_OPROF=1
#export CCC_OPROF_PARAM="--event=CPU_CLK_UNHALTED:6000"

#Perform compilation only (no run).
#export CCC_NO_RUN=1

#Repeat execution a number of times with the same dataset to check execution time variation on the system.
export CCC_RUNS=1

#Use time-run to kill application if it runs for too long
#The reason is that during interative compilation some produced binaries
#are corrupt and have infinite loops.
export CCC_TIMED_RUN="timed-run 3000"

#Architecture specific optimization flags
#export CCC_OPT_PLATFORM="-mA7 -ffixed-r12 -ffixed-r16 -ffixed-r17 -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25"
#export CCC_OPT_PLATFORM="-mA7"
#export CCC_OPT_PLATFORM="-mtune=itanium2"
#export CCC_OPT_PLATFORM="-march=athlon64"

#Some compilation info that should be standardized and automated 
#(if you use ARCH_CFG and/or ARCH_SIZE, you should set CCC_OPT_PLATFORM to "" or other platform related flag
export CCC_OPT_PLATFORM="-msse2"
#export CCC_ARCH_CFG="l1_cache=203; l2_cache=35;"
#export CCC_ARCH_SIZE=132

#Some compilation info that should be standardized and automated
#export CCC_OPT_FINE="loop_tiling=10;"
#export CCC_OPT_PAR_STATIC="all_loops=parallelizable;"

#Some run-time info that eventually should be standardized and automated
#export CCC_RUN_POWER=10
#export CCC_RUN_ENERGY=20
#export CCC_PAR_DYNAMIC="no deps"

#HERE YOU CAN SUBSTITUTE PLATFORM/ENVIRONMENT IDS IF YOU WANT TO DO CROSS-COMPILATION/ANALYSIS
#export CCC_PLATFORM_ID=
#export CCC_ENVIRONMENT_ID=

#Select which processor to run application on, in case of multiprocessor system
#export CCC_PROCESSOR_NUM=

#Select runtime environment (VM or simulator)
#export CCC_RUN_RE=llvm25

#For SPEC2006 and ICI ...
export ICI_WORK_DIR=$PWD/

#Baseline run
#export CCC_NOTES="baseline compilation"
ccc-comp gcc422 -O3
#export CCC_NOTES="baseline run"
ccc-run 1 1

#Optimization run
#export CCC_NOTES="opt compilation"
ccc-comp gcc422 -O2
#export CCC_NOTES="opt run"
ccc-run 1 0
