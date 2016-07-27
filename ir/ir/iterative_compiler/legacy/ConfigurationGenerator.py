
'''
This file generates a list of compiler confogurations that can be applied to each program in
a benchmark dataset. The size of the list that it generates is user defined. This list can then
be used in conjunction with an iterative compilation framework
'''


#!/usr/bin/env python

import pandas as pd
import random as rand
import ConfigurationParser as ConfigurationParser

def generate_optimistaion_specific_flags(flag_database_location):

	## Load Database
	flag_database = pd.read_csv(flag_database_location)

	## Read the database
	flags = flag_database['Flag'].dropna()
	optimisation_levels = ['O1', 'O2', 'O3']

	return {optimisation_level:
			generate_list(flags, flag_database[optimisation_level].dropna(), optimisation_level)
			for optimisation_level in optimisation_levels}

def generate_configuration_list(optimisation_specific_flags, iterative_compilation_depth):

	## Generate 1000 random configurations
	return [generate_random_configuration(rand.choice(optimisation_specific_flags.values()))
			for index in range(0, iterative_compilation_depth)]

def generate_list(flags, opt_list, opt_level):
	flags = [flag for flag, active in zip(flags, opt_list) if int(active) == 1]
	flags.insert(0, "-" + opt_level)
	return flags

def generate_random_configuration(flag_list):
	flags = [flag for flag in flag_list[1: len(flag_list)] if rand.uniform(0, 1) > 0.5]
	flags.insert(0, flag_list[0])
	return flags

