
import os
import pickle

def binary_flag_reader(directory, subdirectory):
	if os.path.isfile(os.path.join(source_directory, directory, "compilation_report.pickle")):
		with open(os.path.join(source_directory, directory, "compilation_report.pickle"), 'rb') as input_file:
			compilation_report = pickle.load(input_file)

		flags = compilation_report['Configuration'][1:]
		return flags
	return False

def optimisation_flag_reader(directory, subdirectory):
	if os.path.isfile(os.path.join(source_directory, directory, "compilation_report.pickle")):
		with open(os.path.join(source_directory, directory, "compilation_report.pickle"), 'rb') as input_file:
			compilation_report = pickle.load(input_file)

		flag = compilation_report['Configuration'][1:]

		if(flag == '-O1'):
			return "one"
		elif(flag == '-O2'):
			return "two"
		else:
			return "three"
	return false