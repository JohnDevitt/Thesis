#!/bin/bash

# Copyright (C) 2004-2009 by Grigori G.Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

##############################################################

#You should look at the end of this script to configure the
#way how to select benchmarks and datasets!!!

COMPILER=gcc422
BASE_OPT="-O3"

BENCH_ITERS=3
OPT_TYPE=0

SEED=11111
ITERS=20
SEQ_LENGTH=10

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

#Use time-run to kill application if it runs for too long.
#The reason is that during interative compilation some produced binaries
#are corrupt and have infinite loops.
export TIMED_RUN="timed-run 3000"

#Architecture specific optimization flags
#export CCC_OPT_PLATFORM="-mA7 -ffixed-r12 -ffixed-r16 -ffixed-r17 -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25"
#export CCC_OPT_PLATFORM="-mA7"
#export CCC_OPT_PLATFORM="-mtune=itanium2"
#export CCC_OPT_PLATFORM="-march=athlon64"

#Some compilation info that should be standardized and automated 
#(if you use ARCH_CFG and/or ARCH_SIZE, you should set CCC_OPT_PLATFORM to "" or other platform related flag
export CCC_OPT_PLATFORM="-msse2"
export CCC_ARCH_CFG="l1_cache=203; l2_cache=35;"
export CCC_ARCH_SIZE=132

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

#Baseline run
#export CCC_NOTES="baseline compilation"
ccc-comp gcc422 -O3
#export CCC_NOTES="baseline run"
#ccc-run 1 1

#Optimization run
#export CCC_NOTES="opt compilation"
#ccc-comp gcc422 -O2
#export CCC_NOTES="opt run"
#ccc-run 1 0


export FILEOUT="$PWD/_stats__misc.txt"
rm -rf $FILEOUT

##############################################################
function experiments
{
  echo "******************************************************************"
  echo "DATASET:" $ds
  echo ""
    
  #Optimization type
  if [ $OPT_TYPE -eq "0" ]; then
   echo "Only base optimization"
   ccc-comp $COMPILER $BASE_OPT
   ccc-run $ds 1

  elif [ $OPT_TYPE -eq "1" ]; then
   echo "Main optimization levels"
   ccc-comp $COMPILER $BASE_OPT
   #first run - reference run to calculate speedup
   ccc-run $ds 1

   ccc-comp $COMPILER "-O2"
   ccc-run $ds 0

   ccc-comp $COMPILER "-O1"
   ccc-run $ds 0

   ccc-comp $COMPILER "-O0"
   ccc-run $ds 0

   ccc-comp $COMPILER "-Os"
   ccc-run $ds 0

  elif [ $OPT_TYPE -eq "2" ]; then
   echo "Optimization flags one by one"
   ccc-run-glob-flags-one-by-one 1 $COMPILER $BASE_OPT $ds

  elif [ $OPT_TYPE -eq "3" ]; then
   echo "Uniform random selection of optimization flags"
   ccc-run-glob-flags-rnd-uniform $ITERS $COMPILER $BASE_OPT $SEED $ds
   let SEED=$SEED+1

  elif [ $OPT_TYPE -eq "4" ]; then
   echo "Random selection of optimization flags (fixed length of the sequence)"
   ccc-run-glob-flags-rnd-fixed $ITERS $SEQ_LENGTH $COMPILER $BASE_OPT $SEED $ds

  elif [ $OPT_TYPE -eq "5" ]; then
   #NEEDS to be updated - get best flags for current program + remove based on MD5, etc
   echo "Randomly remove flags one by one from the optimization sequence"
   ccc-run-glob-flags-one-off-rnd "-O3 -fsched-stalled-insns-dep=19 -fsched-stalled-insns=1 -finline-limit=347 -fbranch-count-reg -fno-branch-target-load-optimize -fno-caller-saves -fno-cse-follow-jumps -fcse-skip-blocks -fno-defer-pop -fdefer-pop -fexpensive-optimizations -fno-force-addr -fno-if-conversion -fif-conversion2 -fno-ivopts -fmerge-constants -foptimize-sibling-calls -fpeephole2 -fprefetch-loop-arrays -fno-regmove -fno-sched-interblock -fsched-spec-load-dangerous -fno-sched-spec -fsched2-use-superblocks -fsplit-ivs-in-unroller -fstrict-aliasing -fthread-jumps -ftree-copyrename -ftree-dominator-opts -ftree-dse -fno-tree-loop-im -fno-tree-loop-linear -ftree-loop-optimize -ftree-pre -fno-tree-sra -fno-tree-vect-loop-version -funroll-loops" $COMPILER $BASE_OPT $SEED $ds

  elif [ $OPT_TYPE -eq "6" ]; then
   echo "Run selected flasgs"
   #reference run to calculate speedups
   ccc-comp $COMPILER $BASE_OPT
   ccc-run $ds 1
   
   exec 9< $CURDIR/ccc--opt-flags.txt
   while read <&9 line1; 
   do 
    ccc-comp $COMPILER "$line1"
    ccc-run $ds 0
   done
   exec 9>&-

  elif [ $OPT_TYPE -eq "7" ]; then
   echo ""

  elif [ $OPT_TYPE -eq "8" ]; then
   echo ""

  elif [ $OPT_TYPE -eq "99" ]; then
   #DUMMY
   
   #SELECT CTDB
   export CCC_URL1=$CCC_URL
   export CCC_DB1=$CCC_DB
   export CCC_USER1=$CCC_USER
   export CCC_PASS1=$CCC_PASS

   export CCC_URL=$CCC_CT_URL
   export CCC_DB=$CCC_CT_DB
   export CCC_USER=$CCC_CT_USER
   export CCC_PASS=$CCC_CT_PASS

   export CCC_CASES=16
   get-best-flag-time $prog_id $CCC_PLATFORM_ID $CCC_ENVIRONMENT_ID $COMPILER_ID $ds

   export CCC_URL=$CCC_URL1
   export CCC_DB=$CCC_DB1
   export CCC_USER=$CCC_USER1
   export CCC_PASS=$CCC_PASS1

   #Get stats
   #finp="_stats__best_flags_time.xrt.txt"
   #finp="_stats__best_flags_time.xrtc.txt"
   #finp="_stats__best_flags_time.xut.txt"
   finp="_stats__best_flags_time.xutc.txt"
   
   exec 5< $finp
   read <&5 X_SPEEDUP 
   read <&5 X_OPT_FLAGS_PLATFORM
   read <&5 X_OPT_FLAGS
   read <&5 X_RUN_ID
   exec 5>&- 

   if [ -n "$X_SPEEDUP" ]; then

     echo ""
     echo "Read from file:"
     echo ""
     echo "BENCH              =" $bench
     echo "SPEEDUP            =" $X_SPEEDUP
     echo "OPT_FLAGS_PLATFORM =" $X_OPT_FLAGS_PLATFORM
     echo "OPT_FLAGS          =" $X_OPT_FLAGS
     echo "RUN_ID             =" $X_RUN_ID

     echo "****************************************************" >> $FILEOUT
     echo "BENCH              =" $bench >> $FILEOUT
     echo "SPEEDUP            =" $X_SPEEDUP >> $FILEOUT
     echo "OPT_FLAGS_PLATFORM =" $X_OPT_FLAGS_PLATFORM >> $FILEOUT
     echo "OPT_FLAGS          =" $X_OPT_FLAGS >> $FILEOUT
     echo "RUN_ID             =" $X_RUN_ID >> $FILEOUT
     export CCC_OPT_PLATFORM=$X_OPT_FLAGS_PLATFORM
     #Run baseline
     ccc-comp $COMPILER "$BASE_OPT"
     ccc-run $ds 0

     exec 5< tmp-ccc-time
     read <&5 TIMEA1
     read <&5 TIMEA2
     exec 5>&- 
     TIMEA=`echo "scale=2;$TIMEA1+$TIMEA2" | bc`

     #Run new
     ccc-comp $COMPILER "$X_OPT_FLAGS"
     ccc-run $ds 1

     exec 5< tmp-ccc-time
     read <&5 TIMEB1
     read <&5 TIMEB2
     exec 5>&- 
     TIMEB=`echo "scale=2;$TIMEB1+$TIMEB2" | bc`

     X_SPEEDUP_NEW=`echo "scale=2;$TIMEA/$TIMEB" | bc`

     echo ""
     echo "Original speedup: " $X_SPEEDUP
     echo "Current  speedup: " $X_SPEEDUP_NEW

     echo "" >> $FILEOUT
     echo "Original speedup: " $X_SPEEDUP >> $FILEOUT
     echo "Current  speedup: " $X_SPEEDUP_NEW >> $FILEOUT

#     ###PROFILE
#     #Run baseline
#     export CCC_GPROF=1
#     ccc-comp $COMPILER "$BASE_OPT"
#     ccc-run $ds 0
#
#     #Run new
#     ccc-comp $COMPILER "$X_OPT_FLAGS"
#     ccc-run $ds 1

      #Prune optimization combination

      ccc-run-glob-flags-one-off-rnd "$X_OPT_FLAGS" $COMPILER $BASE_OPT $SEED 0.02 $ds
    else
      echo "Can't find speedup ..."
    fi
  fi

}

function get_prog_dir
{
  bench=""
  ds_min=0
  ds_max=0
    
  prog=0
  exec 6< $CURDIR/ccc--bench-list.txt
  while <&6 read line ;
  do 
   if [ -n "$line" ]; then
    set -- $line

    bench=$1
    ds_min=$2
    ds_max=$3

    let prog=prog+1
    if [ $prog == $prog_sel ] ; then
     break
    fi
   fi
  done
  exec 6>&-
}

##############################################################
export PATH=.:$PATH

CURDIR=$PWD

. ccc--select-db.sh

#Count programs
prog=0
prog_ds=0
exec 6< $CURDIR/ccc--bench-list.txt
while <&6 read line ;
do 
 if [ -n "$line" ]; then
  set -- $line

  bench=$1
  ds_min=$2
  ds_max=$3

  let dsx=ds_max-ds_min+1

  let prog=prog+1
  let prog_ds=prog_ds+dsx
 fi
done
exec 6>&-

get-compiler-id $COMPILER > tmp99
exec 5< tmp99
read <&5 COMPILER_ID ;
echo $COMPILER_ID
rm -f tmp99

echo "******************************************************************"
echo "PLATFORM ID:                     " $CCC_PLATFORM_ID
echo "ENVIRONMENT ID:                  " $CCC_ENVIRONMENT_ID
echo "COMPILER NAME:                   " $COMPILER
echo "COMPILER ID:                     " $COMPILER_ID
echo "BASE OPTIMIZATION:               " $BASE_OPT
echo ""
echo "NUMBER OF PROGRAMS:              " $prog
echo "NUMBER OF PROGRAMS AND DATASETS: " $prog_ds
echo "******************************************************************"

#MAX ITERATIONS (either all programs or random selection)
p_max=$prog
#p_max=$BENCH_ITERS

for p in `seq 1 $p_max`; do
  #RANDOM SELECTION
  #let prog_sel=($RANDOM*$prog/32768)+1
  #SEQUENTIAL SELECTION
  prog_sel=$p

  get_prog_dir

  echo Processing "${bench}"-tmp ...
  cd  $CURDIR/${bench}-tmp;

  exec 9< _ccc_program_id
  read <&9 prog_id
  read <&9 prog_short_name

  exec 9< _ccc_info_datasets
  read <&9 ds_max_all

  let dsx=ds_max-ds_min+1

  echo "#####################################################################"
  echo "Working directory:  " $PWD
  echo "PROGRAM_ID:         " $prog_id
  echo "PROGRAM SHORT NAME: " $prog_short_name
  echo ""
  echo "DATASET MIN:        " $ds_min
  echo "DATASET MAX:        " $ds_max
  echo "DATASET MAX ALL:    " $ds_max_all
  echo "#####################################################################"

  #RANDOM SELECTION
  #let ds=($RANDOM*$dsx/32768)+ds_min
  #SEQUENTIAL SELECTION (can be just one dataset !=1 )
  for ds in `seq $ds_min $ds_max`; do
  #ONE DATASET (better use updated list with benchmarks with one deafult dataset)
  #let ds=1 

  #For SPEC2006 and ICI ...
  export ICI_WORK_DIR=$PWD/

    experiments

  done
done
