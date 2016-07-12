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

export LD_LIBRARY_PATH=$BUILD_DIR/lib:$BUILD_DIR/lib64:$LD_LIBRARY_PATH

echo "****** Building GMP ..."
mkdir ${OBJ_DIR}-gmp
cd ${OBJ_DIR}-gmp
$SRC_DIR/src-third-party/$GMP_DIR/configure --prefix=$BUILD_DIR --enable-cxx
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
make clean
 if [ "${?}" != "0" ] ; then
   echo "Error: Compilation failed at $PWD!" 
   exit
 fi
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

echo "****** Building MPFR ..."
mkdir ${OBJ_DIR}-mpfr
cd ${OBJ_DIR}-mpfr
$SRC_DIR/src-third-party/$MPFR_DIR/configure --prefix=$BUILD_DIR --with-gmp=$BUILD_DIR
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
make clean
 if [ "${?}" != "0" ] ; then
   echo "Error: Compilation failed at $PWD!" 
   exit
 fi
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

echo "****** Building MPC ..."
mkdir ${OBJ_DIR}-mpc
cd ${OBJ_DIR}-mpc
$SRC_DIR/src-third-party/$MPC_DIR/configure --prefix=$BUILD_DIR --with-gmp=$BUILD_DIR --with-mpfr=$BUILD_DIR

 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
make clean
 if [ "${?}" != "0" ] ; then
   echo "Error: Compilation failed at $PWD!" 
   exit
 fi
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

echo "****** Building PPL ..."
mkdir ${OBJ_DIR}-ppl
cd ${OBJ_DIR}-ppl
CFLAGS="-I$BUILD_DIR/include -fpermissive" CXXFLAGS="-I$BUILD_DIR/include -fpermissive" LDFLAGS=-L$BUILD_DIR/lib $SRC_DIR/src-third-party/$PPL_DIR/configure --prefix=$BUILD_DIR --with-libgmp-prefix=$BUILD_DIR
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
make clean
 if [ "${?}" != "0" ] ; then
   echo "Error: Compilation failed at $PWD!" 
   exit
 fi
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

echo "****** Building CLOOG ..."
mkdir ${OBJ_DIR}-cloog
cd ${OBJ_DIR}-cloog
pushd $SRC_DIR/src-third-party/$CLOOG_DIR
./autogen.sh
popd
CFLAGS=-I$BUILD_DIR/include LDFLAGS=-L$BUILD_DIR/lib $SRC_DIR/src-third-party/cloog/configure --prefix=$BUILD_DIR --without-polylib --with-gmp=$BUILD_DIR --with-ppl=$BUILD_DIR
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
make clean
 if [ "${?}" != "0" ] ; then
   echo "Error: Compilation failed at $PWD!" 
   exit
 fi
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

echo "****** Building GCC ..."
mkdir ${OBJ_DIR}-gcc
cd ${OBJ_DIR}-gcc
#$SRC_DIR/$GCC_DIR/configure --enable-languages=c,c++,fortran --prefix=$BUILD_DIR --with-mpfr=$BUILD_DIR --with-mpc=$BUILD_DIR --with-gmp=$BUILD_DIR --with-ppl=$BUILD_DIR --with-cloog=$BUILD_DIR
$SRC_DIR/$GCC_DIR/configure --prefix=$BUILD_DIR --with-mpfr=$BUILD_DIR --with-mpc=$BUILD_DIR --with-gmp=$BUILD_DIR --with-ppl=$BUILD_DIR --with-cloog=$BUILD_DIR
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
make clean
 if [ "${?}" != "0" ] ; then
   echo "Error: Compilation failed at $PWD!" 
   exit
 fi
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
