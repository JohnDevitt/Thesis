
#!/usr/bin/env python

import pickle
import os
import math
import pandas as pd 


def parse_all_runtimes(source_directory):

	subdirectories = [ name for name in os.listdir(source_directory)
			if os.path.isdir(os.path.join(source_directory, name)) ]

	program_runtimes = []

	for directory in subdirectories:
		if os.path.isfile(os.path.join(source_directory, directory, "compilation_report.pickle")):
			with open(os.path.join(source_directory, directory, "compilation_report.pickle"), 'rb') as input_file:
				compilation_report = pickle.load(input_file)
				if(	not math.isinf(compilation_report['Baseline']) or
					not math.isinf(compilation_report['Best']) ):
					for runtime in compilation_report['Runtimes']:
						difference = compilation_report['Baseline'] - runtime
						proportion = (difference / compilation_report['Baseline']) * 100
						program_runtimes.append((proportion, directory))


	best_runtimes, directories = zip(*program_runtimes)
	dataframe = pd.DataFrame({"Decrease": best_runtimes, "Program": directories})

	return dataframe

def parse_best_runtimes(source_directory):

	subdirectories = [ name for name in os.listdir(source_directory)
			if os.path.isdir(os.path.join(source_directory, name)) ]

	program_runtimes = []

	for directory in subdirectories:
		if os.path.isfile(os.path.join(source_directory, directory, "compilation_report.pickle")):
			with open(os.path.join(source_directory, directory, "compilation_report.pickle"), 'rb') as input_file:
				compilation_report = pickle.load(input_file)
				if(	not math.isinf(compilation_report['Baseline']) or
					not math.isinf(compilation_report['Best']) ):
					difference = compilation_report['Baseline'] - compilation_report['Best']
					proportion = (difference / compilation_report['Baseline']) * 100
					program_runtimes.append((proportion, directory))

	program_runtimes.sort(key = lambda t: t[0])

	best_runtimes, directories = zip(*program_runtimes)
	dataframe = pd.DataFrame({"Decrease": best_runtimes, "Program": directories})

	return dataframe

	