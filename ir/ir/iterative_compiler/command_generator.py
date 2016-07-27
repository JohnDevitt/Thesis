
#!/usr/bin/env python

import random as rand
import os

def parse_configuration(configuration, directory):

	configuration_string = ""
	for flag in configuration:
		configuration_string += str(flag) + " "

	return generate_compile_command(configuration_string, directory)

def generate_compile_command(configuration, directory):

	compile_command = "make -C "
	compile_command += directory 
	compile_command += "/ clean; make -C "
	compile_command += directory
	compile_command += "/ CFLAGS='"
	compile_command += configuration
	compile_command += "'"

	return compile_command

def generate_run_command(directory, filename):

	return os.path.join(directory, filename)