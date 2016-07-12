#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2007-2010, UNIDAPT Group, INRIA, France (http://unidapt.org)

source ./___common_environment.sh
source ./__clean.sh

#Select only a few functions to process
export ICI_FILE_SELECT_FUNCTIONS=$PWD/_ctuning_select_functions.txt

#Change optimization level for the analysis compiler
#(normally should be used with the removal of optimization flags)
#export CTUNING_ANALYSIS_OPT_LEVEL=-O2

#remove optimization flags specified in the following file when processing CMD
#(to avoid interference between original and predicted optimizations)
export CTUNING_FILE_REMOVE_OPT_FLAGS=$PWD/_ctuning_remove_opt_flags.gcc44p

ctuning-fortran -O3 --ct-extract-structure *.f
