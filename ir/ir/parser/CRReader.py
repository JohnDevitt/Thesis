
import os
import inflect


def binary_flag_reader(directory, subdirectory):
	filepath = os.path.join(directory, subdirectory, "compilation_report.txt")
	if os.path.exists(filepath):
		compilation_report = open(filepath, "r")
		compilation_vector = compilation_report.readlines()
		flag_vector = compilation_vector[7].split()

		p = inflect.engine()

		#O_flag = p.number_to_words(flag_vector[8][10:])
		compiler_configuration = flag_vector[9: len(flag_vector) - 2]

		#compiler_configuration.insert(0, O_flag)
		return compiler_configuration

def optimisation_flag_reader(directory, subdirectory):
	filepath = os.path.join(directory, subdirectory, "compilation_report.txt")
	if os.path.exists(filepath):
		compilation_report = open(filepath, "r")
		compilation_vector = compilation_report.readlines()
		flag_vector = compilation_vector[7].split()

		p = inflect.engine()

		#O_flag = p.number_to_words(flag_vector[8][10:])
		compiler_configuration = int(flag_vector[8][10:])

		if(compiler_configuration == 1):
			return "one"
		elif(compiler_configuration == 2):
			return "two"
		else:
			return "three"

		#compiler_configuration.insert(0, O_flag)
		#return compiler_configuration