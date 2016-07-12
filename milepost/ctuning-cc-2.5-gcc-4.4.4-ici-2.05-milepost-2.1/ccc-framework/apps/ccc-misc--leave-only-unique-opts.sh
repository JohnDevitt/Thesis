#!/bin/bash

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

in=ccc--opt-flags.txt
out=ccc--opt-flags-unique.txt

rm -f $out

cat $in | while read line; 
do 
 if [ -n "$line" ]; then
  echo Processing "${line} ..."

  unique=1
  
  exec 9< $out
   while read <&9 line1; 
   do 
    if [ "$line" = "$line1" ] ; then
     unique=0;
     break;
    fi
   done  

  if [ $unique -eq "1" ] ; then
    echo ${line} >> $out
    echo "  unique..." 
  fi
 fi
done
