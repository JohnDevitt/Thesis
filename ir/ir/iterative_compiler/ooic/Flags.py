
'''
This file generates a list of compiler confogurations that can be applied to each program in
a benchmark dataset. The size of the list that it generates is user defined. This list can then
be used in conjunction with an iterative compilation framework
'''


#!/usr/bin/env python

import pandas as pd

class Flags:
	def __init__(self, flag_database_path):
		self.flag_database_path = flag_database_path
		self.optimisations = ['O1', 'O2', 'O3']

	def generate_flag_list(self):
		flag_database = pd.read_csv(self.flag_database_path)
		self.flag_list = flag_database['Flag'].dropna()

	def generate_flag_map(self):
		flag_database = pd.read_csv(self.flag_database_path)
		self.flag_map = {}

		for optimisation in self.optimisations:
			self.flag_map[optimisation] = []
			for (value, flag) in zip(flag_database[optimisation].dropna(), flag_database['Flag'].dropna()):
				if(int(value) == 1):
					self.flag_map[optimisation].append(flag)



'''

def generate_flags(flag_database_location):

	## Load Database
	flag_database = pd.read_csv(flag_database_location)

	## Read the database
	flags = flag_database['Flag'].dropna()
	optimisations = ['O1', 'O2', 'O3']

	return {optimisation:
			generate_flag_list(flags, flag_database[optimisation].dropna(), optimisation)
			for optimisation in optimisations}



def generate_configurations(flags, iterative_compilation_depth):

	## Generate 1000 random configurations
	return [generate_configuration(rand.choice(flags.values()))
			for index in range(0, iterative_compilation_depth)]

def generate_flag_list(flags, optimisations, optimisation):
	flags = [flag for flag, active in zip(flags, optimisations) if int(active) == 1]
	flags.insert(0, "-" + optimisation)
	return flags

def generate_configuration(flag_list):
	flags = [flag for flag in flag_list[1: len(flag_list)] if rand.uniform(0, 1) > 0.5]
	flags.insert(0, flag_list[0])
	return flags

'''