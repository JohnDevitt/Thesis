#!/bin/bash -x

#INSTALL=$PWD/../../../../../install
INSTALL=$COMPILER_PATH

# Path to the patched GCC
export LD_LIBRARY_PATH=$INSTALL/lib:$INSTALL/lib64:$LD_LIBRARY_PATH
CC=$INSTALL/bin/gcc

# GCC-ICI flag
MISC_OPTS='-O3 -lm -fici'

export ICI_VERBOSE=1
export ICI_PLUGIN_VERBOSE=1
export ICI_PLUGIN="$ICI_LIB/${ICI_PLUGIN_VER}-substitute-passes.so"

make COMPILER=${CC} MISC_OPTS="${MISC_OPTS}"

export -n ICI_PLUGIN
