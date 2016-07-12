#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2007-2010, UNIDAPT Group, INRIA, France (http://unidapt.org)

source ./___common_environment.sh
source ./__clean.sh

#CTUNING_FLAGS=--ct-opt=23935585718738062
#CTUNING_FLAGS=--ct-opt=11475790782770590
CTUNING_FLAGS=--ct-opt=23011215880571251

./configure CC=ctuning-cc CFLAGS=$CTUNING_FLAGS CXX=ctuning-c++ CXXFLAGS=$CTUNING_FLAGS F77=ctuning-fortran FFLAGS=$CTUNING_FLAGS
