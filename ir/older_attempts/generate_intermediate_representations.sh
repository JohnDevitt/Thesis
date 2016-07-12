#!/usr/bin/env bash


for d in */ ; do
	cd $d;
	make clean; make CFLAGS='-fdump-tree-all'	
	cd ..;
done

