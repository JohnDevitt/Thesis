#!/bin/bash

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2007-2010, UNIDAPT Group, INRIA, France

# directory names
BUILD_EXT=install
BUILD_DIR=$PWD/$BUILD_EXT
OBJ_DIR=$BUILD_DIR/obj
PWD_SAVE=$PWD

# remove old dir (remark if recompilation)
rm -rf $BUILD_DIR
mkdir $BUILD_DIR

cd $PWD_SAVE
. ./_build_gcc.sh

cd $PWD_SAVE
. ./_build_ccc_minimal.sh

cd $PWD_SAVE
. ./_build_plugins.sh

cd $PWD_SAVE
. ./_build_plugins_ml.sh
