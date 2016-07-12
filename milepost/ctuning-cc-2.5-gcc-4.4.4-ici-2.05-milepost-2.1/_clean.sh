#!/bin/bash

# Copyright (C) 2007-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

# directory names
BUILD_EXT=install
BUILD_DIR=$PWD/$BUILD_EXT

echo "Would you like to remove all files (Y/N)"
read var
 
if [ "$var" = "Y" ] || [ "$var" = "y" ] ; then
 rm -rf $BUILD_DIR
fi
