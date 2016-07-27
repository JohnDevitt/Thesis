
#!/usr/bin/env python

from Flags import Flags
from ConfigurationList import ConfigurationList
from Program import Program
import os

def main():
	programs = load_programs();

	for program in programs:
		program.find_best_configuration


def load_programs():

	## Generate the flags.
	flags = Flags('/home/john/Thesis/ir/ir/arm-none-eabi-gcc-4.8.2-flags.csv')
	flags.generate_flag_list()
	flags.generate_flag_map()

	## Generate and initial list of configurations.
	configurations = ConfigurationList()
	configurations.build_configurations(5, flags)

	## Load the programs
	source_directory = '/home/john/Thesis/beebs/src'
	subdirectories = [ name for name in os.listdir(source_directory)
			if os.path.isdir(os.path.join(source_directory, name)) ]

	programs = [Program(source_directory, subdirectory) for subdirectory in subdirectories]

	for program in programs:
		for configuration in configurations.configurations:
			program.configurations.append(configuration)

	return programs

if __name__ == '__main__':
	main()