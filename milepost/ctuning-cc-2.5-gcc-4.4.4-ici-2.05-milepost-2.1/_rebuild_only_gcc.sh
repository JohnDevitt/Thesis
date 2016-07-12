#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2007-2010, UNIDAPT Group, INRIA, France

# change this value to match the number of cores in your build system
PARALLEL="-j 2"

# directory names
GMP_DIR=gmp-5.0.1
MPFR_DIR=mpfr-2.4.2
MPC_DIR=mpc-0.8.1
PPL_DIR=ppl-0.10.2-modified
CLOOG_DIR=cloog
GCC_DIR=milepost-gcc-4.4.4
SRC_DIR=$PWD
BUILD_EXT=install
BUILD_DIR=$PWD/$BUILD_EXT
OBJ_DIR=$BUILD_DIR/obj

# remove old dir (remark if recompilation)
#rm -rf $BUILD_DIR
#mkdir $BUILD_DIR

export LD_LIBRARY_PATH=$BUILD_DIR/lib:$BUILD_DIR/lib64:$LD_LIBRARY_PATH

cd ${OBJ_DIR}-gcc
make ${PARALLEL}
 if [ "${?}" != "0" ] ; then
   echo "Error: Compilation failed at $PWD!" 
   exit
 fi
make install
 if [ "${?}" != "0" ] ; then
   echo "Error: Compilation failed at $PWD!" 
   exit
 fi
