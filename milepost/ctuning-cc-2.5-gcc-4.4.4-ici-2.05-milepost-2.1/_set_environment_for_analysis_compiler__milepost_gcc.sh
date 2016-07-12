#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France

BUILD_EXT=install
BUILD_DIR=$PWD/$BUILD_EXT
PWD_SAVE=PWD

export PATH=$BUILD_DIR/bin:$PATH
export LD_LIBRARY_PATH=$BUILD_DIR/lib:$BUILD_DIR/lib64:$LD_LIBRARY_PATH

#set MILEPOST compiler names (in the future it may be some other compiler
#that supports ICI and plugins)
export CTUNING_ANALYSIS_CC=gcc
export CTUNING_ANALYSIS_CPP=g++
export CTUNING_ANALYSIS_FORTRAN=gfortran

#ICI PLUGINS
export ICI2_PLUGIN_VER=gcc-plugin-ici2
export ICI_LIB=$BUILD_DIR/lib
export ICI_PLUGIN_VER=$ICI2_PLUGIN_VER

export CCC_ICI_USE=ICI_USE
export CCC_ICI_PLUGINS=ICI_PLUGIN

export CCC_ICI_PASSES_FN=ici_passes_function
export CCC_ICI_PASSES_EXT=.txt

#export CCC_ICI_PASSES_RECORD_PLUGIN=$ICI_LIB/$ICI_PLUGIN_VER-save-executed-passes.so
export CCC_ICI_PASSES_RECORD_PLUGIN=$ICI_LIB/$ICI_PLUGIN_VER-extract-program-structure.so
export CCC_ICI_FEATURES_ST_FN=ici_features_function
export CCC_ICI_FEATURES_ST_EXT=.ft
export CCC_ICI_FEATURES_ST_EXTRACT_PLUGIN=$ICI_LIB/$ICI_PLUGIN_VER-extract-program-static-features.so

export ICI_PROG_FEAT_PASS=fre

export ML_ST_FEAT_TOOL=$BUILD_DIR/bin/featlstn.P
export XSB_DIR="$BUILD_DIR/3.2"
export ICI_PROG_FEAT_EXT_TOOL=$BUILD_DIR/bin/ml-feat-proc

export CCC_ROOT=$PWD/ccc-framework

export CCC_PLUGINS=$CCC_ROOT/src-plat-indep/
export PATH=$CCC_ROOT/src-plat-indep/plugins:$PATH

export CCC_UUID=uuidgen
