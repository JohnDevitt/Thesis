#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2007-2010, UNIDAPT Group, INRIA, France (http://unidapt.org)

source ./___common_environment.sh
source ./__clean.sh

if [ -z "$CCC_CTS_PASS" ] ; then
 read -s -p "  Enter cTuning password for $CCC_CTS_USER : " pass
 export CCC_CTS_PASS=$pass
 echo ""
fi

#remove optimization flags specified in the following file when processing CMD
#(to avoid interference between original and predicted optimizations)
export CTUNING_FILE_REMOVE_OPT_FLAGS=$PWD/_ctuning_remove_opt_flags.gcc44p

#you can select an optimzation case from http://cTuning.org/cdatabase
#milepost-gcc --ct-opt=23935585718738062 *.c
#milepost-gcc --ct-opt=11475790782770590 *.c
ctuning-c++ --ct-opt=23011215880571251 -O3 *.cc
