#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2007-2010, UNIDAPT Group, INRIA, France (http://unidapt.org)

source ./___common_environment.sh
source ./__clean.sh

#Trigger transparent program structure extraction and aggregate info about compiled program
export CTUNING_EXTRACT_STRUCTURE=$PWD/_ctuning_program_structure.txt

#remove optimization flags specified in the following file when processing CMD
#(to avoid interference between original and predicted optimizations)
export CTUNING_FILE_REMOVE_OPT_FLAGS=$PWD/_ctuning_remove_opt_flags.gcc44p

#files are removed since they will be appended
rm -f $CTUNING_EXTRACT_STRUCTURE

#substitute original optimizations
export CCC_OPTS=-"O2 -funroll-loops"

ctuning-cc -O3 -DXYZ -funroll-all-loops *.c
