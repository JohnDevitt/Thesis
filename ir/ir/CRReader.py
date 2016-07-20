
import os
import inflect


def main(directory, subdirectory):
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

if __name__ == '__main__':
	main()