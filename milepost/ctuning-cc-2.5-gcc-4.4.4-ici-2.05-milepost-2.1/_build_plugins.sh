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

export LD_LIBRARY_PATH=$BUILDDIR/lib:$BUILDDIR/lib64:$LD_LIBRARY_PATH

#go to all dirs recursively
GLB_VAR=0;
GLB_DEST="";
addinclude="";

function recursive
{
 cd "$1" 2> /dev/null
 GLB_VAR=$?
 if [ $GLB_VAR -eq 0 ]
 then

  if [ -f "Makefile" ] ; then
    echo "============================================================="
    echo "Compiling $PWD ..."
    echo

    make clean ICI_PLUGIN_VER=$ICI2_PLUGIN_VER
     if [ "${?}" != "0" ] ; then
      echo "Error: Compilation failed at $PWD!" 
      exit
     fi

    make MISCOPT=-O3 EXTRAINCLUDE="-I$SRC_DIR/$GCC_DIR -I$BUILD_DIR/include" EXTRALIBRARY=-L$BUILD_DIR/lib ICI_PLUGIN_VER=$ICI2_PLUGIN_VER
    if [ "${?}" != "0" ] ; then
     echo ""
     echo "Error: Compilation failed!" 
     echo "Check $PWD for problems ..."
     rm -f *.tmp
     exit
    fi
    make install INSTALL_DIR=$BUILD_DIR/lib ICI_PLUGIN_VER=$ICI2_PLUGIN_VER
    make clean ICI_PLUGIN_VER=$ICI2_PLUGIN_VER
  fi 

  for i in *
  do
   if [ "$i" = "*" -o -h "$i" -o -c "$i" ] ; then
    continue
   fi
   if [ -d "$i" -a -r "$i" ] ; then
    recursive "$i"
    if [ $GLB_VAR -eq 0 ] ; then
     cd ..
    else
     GLB_VAR=0;
    fi
   else
    #$i - filename

    continue
   fi
  done
 fi
}

recursive $SRC_DIR/$PLUGIN_DIR/src
