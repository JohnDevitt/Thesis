#!/usr/bin/env python

import os
import subprocess

def main(source_directory, subdirectory):
	
	#configure = os.path.join(base_directory, "configure")
	#make = "cd " + base_directory + " && make"

	#print subprocess.Popen(configure, shell=True, stdout=subprocess.PIPE).stdout.read()
	#print subprocess.Popen(make, shell=True, stdout=subprocess.PIPE).stdout.read()

	#print "CONFIGURED AND MADE"

	directory = os.path.join(source_directory, subdirectory)
	generate_command = "make -C " + directory + "/ clean; make -C " + directory + "/ CFLAGS='-fdump-tree-cfg'"
	print subprocess.Popen(generate_command, shell=True, stdout=subprocess.PIPE).stdout.read()

	filename = subdirectory + ".c.011t.cfg"
	filepath = os.path.join(source_directory, subdirectory, filename)
	if os.path.exists(filepath):
		return True
	else:
		return False

if __name__ == '__main__':
	main()
