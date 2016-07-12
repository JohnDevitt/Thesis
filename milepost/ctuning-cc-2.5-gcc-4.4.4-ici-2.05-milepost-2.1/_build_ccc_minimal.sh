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

cd ccc-framework/src-plat-dep/tools/ctuning-cc
make
if [ "${?}" != "0" ] ; then
  echo "Error: Compilation failed at $PWD!" 
  exit
fi
make install DEST=$BUILD_DIR
if [ "${?}" != "0" ] ; then
  echo "Error: Compilation failed at $PWD!" 
  exit
fi
cd $PWD_SAVE

cd ccc-framework/src-plat-dep/plugins/ml/ccc-ml-accumulate-features
make
if [ "${?}" != "0" ] ; then
  echo "Error: Compilation failed at $PWD!" 
  exit
fi
make install DEST=$BUILD_DIR
if [ "${?}" != "0" ] ; then
  echo "Error: Compilation failed at $PWD!" 
  exit
fi
cd $PWD_SAVE
