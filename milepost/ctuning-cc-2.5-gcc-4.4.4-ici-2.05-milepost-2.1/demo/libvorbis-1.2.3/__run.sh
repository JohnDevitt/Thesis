#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2007-2010, UNIDAPT Group, INRIA, France (http://unidapt.org)

source ./___common_environment.sh

#Select only a few functions to process when predicting optimizations
#export ICI_FILE_SELECT_FUNCTIONS=$PWD/_ctuning_select_functions.txt

time ./__run1.sh

echo ""
echo "Output:"
more ftmp_out