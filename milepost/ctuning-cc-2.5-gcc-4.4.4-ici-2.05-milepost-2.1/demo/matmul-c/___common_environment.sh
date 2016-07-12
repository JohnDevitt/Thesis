#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2007-2010, UNIDAPT Group, INRIA, France (http://unidapt.org)

export ICI_PLUGIN_VERBOSE=1
export ICI_VERBOSE=1
export ICI_PROG_FEAT_PASS=fre

#set cTuning web-service parameters
export CCC_CTS_URL=cTuning.org/wiki/index.php/Special:CDatabase?request=
#export CCC_CTS_URL=localhost/cTuning/wiki/index.php/Special:CDatabase?request=
export CCC_CTS_DB=cod_opt_cases
#set cTuning username (self-register at http://cTuning.org/wiki/index.php/Special:UserLogin)
export CCC_CTS_USER=gfursin

#set user compiler (currently gcc, but can be used with any other
# compiler such as LLVM, ICC, XL, ROSE, Open64, etc)
export CTUNING_COMPILER_CC=gcc
export CTUNING_COMPILER_CPP=g++
export CTUNING_COMPILER_FORTRAN=gfortran

#misc parameters - don't change unless you understand what you do!

#compiler which was used to extract features for all programs to keep at cTuning.org
#do not change it unless you understand what you do ;) ...
export CCC_COMPILER_FEATURES_ID=129504539516446542

#use architecture flags from cTuning
export CCC_OPT_ARCH_USE=0

#retrieve opt cases only when execution time > TIME_THRESHOLD
export TIME_THRESHOLD=0.3

#retrieve opt cases only with specific notes
#export NOTES=

#retrieve opt cases only when profile info is !=""
#export PG_USE=1

#retrieve opt cases only when execution output is correct (or not if =0)
export OUTPUT_CORRECT=1

#check user or total execution time
#export RUN_TIME=RUN_TIME_USER
export RUN_TIME=RUN_TIME

#Sort optimization case by speedup (0 - ex. time, 1 - code size, 2 - comp time)
export SORT=012

#produce additional optimization report including optimization space froniters
export CT_OPT_REPORT=1

#Produce optimization space frontier
#export DIM=01 (2D frontier)
#export DIM=02 (2D frontier)
#export DIM=12 (2D frontier)
#export DIM=012 (3D frontier)
#export DIM=012

#Cut cases when producing frontier (select cases when speedup 0,1 or 2 is more than some threshold)
#export CUT=0,0,1.2
#export CUT=1,0.80,1
#export CUT=0,0,1

#find similar cases from the following platform
export CCC_PLATFORM_ID=2111574609159278179
export CCC_ENVIRONMENT_ID=2781195477254972989
export CCC_COMPILER_ID=331350613878705696
