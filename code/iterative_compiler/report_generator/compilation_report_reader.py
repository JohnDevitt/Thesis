
#!/usr/bin/env python

import pickle
import os
import math
import pandas as pd 


def parse_all_runtimes(output_directory):

	subdirectories = [ name for name in os.listdir(os.path.join(output_directory, "reports"))
			if os.path.isdir(os.path.join(output_directory, "reports", name)) ]

	program_runtimes = []

	for directory in subdirectories:
		if os.path.isfile(os.path.join(output_directory, "reports", directory, "compilation_report.pickle")):
			with open(os.path.join(output_directory, "reports", directory, "compilation_report.pickle"), 'rb') as input_file:
				compilation_report = pickle.load(input_file)
				if(	not math.isinf(compilation_report['Baseline']) or
					not math.isinf(compilation_report['Best']) ):
					for runtime in compilation_report['Runtimes']:
						difference = compilation_report['Baseline'] - runtime
						proportion = (difference / compilation_report['Baseline']) * 100
						program_runtimes.append((proportion, directory))


	best_runtimes, directories = zip(*program_runtimes)
	dataframe = pd.DataFrame({"Speedup": best_runtimes, "Program": directories})

	return dataframe

def parse_best_runtimes(output_directory):	


	subdirectories = [ name for name in os.listdir(os.path.join(output_directory, "reports"))
			if os.path.isdir(os.path.join(output_directory, "reports", name)) ]

	program_runtimes = []

	for directory in subdirectories:

		if os.path.isfile(os.path.join(output_directory, "reports", directory, "compilation_report.pickle")):
			with open(os.path.join(output_directory, "reports", directory, "compilation_report.pickle"), 'rb') as input_file:			
				compilation_report = pickle.load(input_file)
				if(	not math.isnan(compilation_report['Baseline']) or
					not math.isnan(compilation_report['Best']) ):
					
					difference = compilation_report['Baseline'] - compilation_report['Best']
					if(compilation_report['Baseline']) != 0:
						proportion = (difference / compilation_report['Baseline']) * 100
						if not math.isnan(proportion):
							program_runtimes.append((proportion, directory))

	program_runtimes.sort(key = lambda t: t[0])

	print program_runtimes

	best_runtimes, directories = zip(*program_runtimes)

	print sum(best_runtimes)/len(best_runtimes)

	dataframe = pd.DataFrame({"Speedup": best_runtimes, "Program": directories})

	return dataframe

	