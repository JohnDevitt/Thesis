
import os
import pickle

def binary_flag_reader(output_directory, subdirectory):
	if os.path.isfile(os.path.join(output_directory, "reports", subdirectory, "compilation_report.pickle")):
		with open(os.path.join(output_directory, "reports", subdirectory, "compilation_report.pickle"), 'rb') as input_file:
			compilation_report = pickle.load(input_file)

		flags = compilation_report['Configuration'][1:]
		return flags
	return False

def optimisation_flag_reader(output_directory, subdirectory):

	if os.path.isfile(os.path.join(output_directory, "reports", subdirectory, "compilation_report.pickle")):
		with open(os.path.join(output_directory, "reports", subdirectory, "compilation_report.pickle"), 'rb') as input_file:
			compilation_report = pickle.load(input_file)

		#print compilation_report

		flag = compilation_report['Configuration'][0]

		if(flag == '-O1'):
			return "one"
		elif(flag == '-O2'):
			return "two"
		else:
			return "three"
	return False