#!/bin/bash

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

rm -f inp-opt-ici-passes.txt

cat ccc--opt-flags.txt | while read line; 
do 
 if [ -n "$line" ]; then
  echo Processing "${line} ..."

  rm -rf ccc-ici-tmp.tmp
  ccc-ici-passes-record ccc-ici-tmp.tmp gcc "${line}"

  i=0
  exec 9< ccc-ici-tmp.tmp
  while read <&9 line1; 
  do 
    if [ $i -ne "0" ] ; then
      echo -n "," >> inp-opt-ici-passes.txt
    fi
    echo -n $line1 >> inp-opt-ici-passes.txt
    let i=1
  done  

  echo "" >> inp-opt-ici-passes.txt
  rm -f ccc-ici-tmp.tmp
 fi
done
