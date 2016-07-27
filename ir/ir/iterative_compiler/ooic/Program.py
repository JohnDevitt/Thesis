
#!/usr/bin/env python

import subprocess
from ConfigurationList import ConfigurationList

class Program:
	def __init__(self, directory, filename):
		self.directory = directory
		self.filename = filename
		self.run_command = os.path.join(directory, filename)
		self.configurations = []
		self.runtimes = []

	def generate_compile_command(index):
		configuration_string = ""
		for flag in configuration:
			configuration_string += str(flag) + " "

		return compile_command = "make -C " + directory + "/ clean; make -C "
				+ directory + "/ CFLAGS='" + configuration + "'"

	def compile(configuration):
		return run_bash_command(self.compile_command)

	def run_bash_command(command):
		p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)
		p.communicate()[0]

		if (p.returncode > 1): 
			return False
		return True

	def add_configuration():


