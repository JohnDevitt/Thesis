#!/bin/bash

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

exec 6< ccc--bench-list.txt
while <&6 read line ;
do 
 if [ -n "$line" ]; then

  set -- $line

  bench=$1
  ds_min=$2
  ds_max=$3

  echo Deleting "${bench}"-tmp ...
  rm -rf ${bench}-tmp;

 else
  echo ""
 fi
done
