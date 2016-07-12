#!/bin/bash -x

#INSTALL=$PWD/../../../../../install
INSTALL=$COMPILER_PATH

# Path to the patched GCC
export LD_LIBRARY_PATH=$INSTALL/lib:$INSTALL/lib64:$LD_LIBRARY_PATH
CC=$INSTALL/bin/gcc

# GCC-ICI flag
MISC_OPTS='-O3 -lm'

export ICI_VERBOSE=1
export ICI_PLUGIN_VERBOSE=1

export XSB_DIR="$INSTALL/3.2"
export ICI_PLUGIN="$ICI_LIB/${ICI_PLUGIN_VER}-extract-program-static-features.so"
export ICI_PROG_FEAT_PASS=fre
export ICI_PROG_FEAT_EXT_TOOL="$INSTALL/bin/ml-feat-proc"

export PATH=$INSTALL/bin:$PATH

export ICI_USE=1
make COMPILER=${CC} MISC_OPTS="${MISC_OPTS}"

export -n ICI_PLUGIN
