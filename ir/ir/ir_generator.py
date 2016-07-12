#!/usr/bin/env python

import os
import subprocess

base_directory = "/home/john/Thesis/beebs"
source_directory = "/home/john/Thesis/beebs/src"
subdirectories = [ name for name in os.listdir(source_directory) if os.path.isdir(os.path.join(source_directory, name)) ]

#configure = os.path.join(base_directory, "configure")
#make = "cd " + base_directory + " && make"

#print subprocess.Popen(configure, shell=True, stdout=subprocess.PIPE).stdout.read()
#print subprocess.Popen(make, shell=True, stdout=subprocess.PIPE).stdout.read()

#print "CONFIGURED AND MADE"


for subdirectory in subdirectories:

	directory = os.path.join(source_directory, subdirectory)
	generate_command = "make -C " + directory + "/ clean; make -C " + directory + "/ CFLAGS='-fdump-tree-ssa'"
	print subprocess.Popen(generate_command, shell=True, stdout=subprocess.PIPE).stdout.read()
