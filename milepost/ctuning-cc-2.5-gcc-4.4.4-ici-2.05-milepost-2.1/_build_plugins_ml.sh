#!/bin/bash

# Copyright (C) 2007-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

# directory names
GCC_DIR=milepost-gcc-4.4.4
PLUGIN_DIR=plugins-ici-2.05
SRC_DIR=$PWD
BUILD_EXT=install
BUILD_DIR=$PWD/$BUILD_EXT
OBJ_DIR=$BUILD_DIR/obj
ICI2_PLUGIN_VER=gcc-plugin-ici2
XSB_VER=3.2

export LD_LIBRARY_PATH=$BUILDDIR/lib:$BUILDDIR/lib64:$LD_LIBRARY_PATH

TMP="tmp.tmp"

#go to all dirs recursively
GLB_VAR=0;
GLB_DEST="";
addinclude="";

#Have to compile XSB in its own directory
pushd $SRC_DIR/src-third-party/XSB/build/
make distclean
./configure --prefix=$BUILD_DIR
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
./makexsb
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
./makexsb install
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
popd

#Get installation dir for XSB (architecture related)
rm -rf $TMP
ls $BUILD_DIR/$XSB_VER/config > $TMP
exec 9< $TMP
read <&9 xsb_dir_add
rm -rf $TMP

echo $xsb_dir_add

cd $SRC_DIR/$PLUGIN_DIR/src-ml/extract-program-static-features.legacy
make clean ICI_PLUGIN_VER=$ICI2_PLUGIN_VER
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
make MISCOPT=-O3 EXTRAINCLUDE=-I$SRC_DIR/$GCC_DIR ICI_PLUGIN_VER=$ICI2_PLUGIN_VER
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
make install INSTALL_DIR=$BUILD_DIR/lib ICI_PLUGIN_VER=$ICI2_PLUGIN_VER
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi

cd $SRC_DIR/$PLUGIN_DIR/src-ml/extract-program-static-features.legacy/ml-feat-proc
make clean ICI_PLUGIN_VER=$ICI2_PLUGIN_VER
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
make XSB_DIR=$BUILD_DIR/$XSB_VER XSB_DIR_ADD=$xsb_dir_add ICI_PLUGIN_VER=$ICI2_PLUGIN_VER
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
make install INSTALL_DIR=$BUILD_DIR/bin ICI_PLUGIN_VER=$ICI2_PLUGIN_VER
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
make clean ICI_PLUGIN_VER=$ICI2_PLUGIN_VER
 if [ "${?}" != "0" ] ; then
   echo "Error: Configuration failed at $PWD!" 
   exit
 fi
