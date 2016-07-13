#!/usr/bin/env python


import os
import csv
import random
import subprocess
import time

def main(source_directory, subdirectory, flag_database, iterative_compilation_depth):

	
	absolute_path = os.path.join(source_directory, subdirectory)
	optimisation_flags = build_flag_lists(flag_database)

	make_system_call("make -C " + absolute_path + "/ clean; make -C " + absolute_path + "/ CFLAGS='-O3'")
	baseline = timed_make_system_call(absolute_path)

	best_run_time = float("inf")
	best_configuration = ""

	worst_run_time = 0
	worst_configuration = ""

	for i in range(1, iterative_compilation_depth):

		compile_command = build_compile_command(source_directory, subdirectory, optimisation_flags)
		make_system_call(compile_command)

		run_command = build_run_command(source_directory, subdirectory)
		run_time = timed_make_system_call(run_command)

		if(run_time < best_run_time):
			best_run_time = run_time
			best_configuration = compile_command

		if(run_time > worst_run_time):
			worst_run_time = run_time
			worst_configuration = compile_command

	write_output_to_file(source_directory, subdirectory, baseline, best_run_time, best_configuration, worst_run_time, worst_configuration)


def build_flag_lists(filepath):
	optimisation_flags = []

	with open(filepath, 'rb') as file:
		reader = csv.reader(file)

		for i in range(2, 5):
			optimisation_flags.append([row[0] for row in reader if row[i] == '1'])
			file.seek(0)

	return optimisation_flags

def build_compile_command(source_directory, subdirectory, optimisation_flags):

	absolute_path = os.path.join(source_directory, subdirectory)

	compile_command = "make -C " + absolute_path + "/ clean; make -C " + absolute_path + "/ CFLAGS='"
	optimisation_level = random.randint(1, 3)

	compile_command += "-O"
	compile_command += str(optimisation_level)
	compile_command += " "

	for flag in optimisation_flags[optimisation_level - 1]:
		if(random.uniform(0, 1) < 0.5):
			compile_command += flag
			compile_command += " "

	compile_command += "'"

	return compile_command

def build_run_command(source_directory, subdirectory):
	return os.path.join(source_directory, subdirectory, subdirectory)

def make_system_call(command):

	print command

	print subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.read()

def timed_make_system_call(command):
	start = time.time()
	make_system_call(command)
	end = time.time()

	return end - start

def	write_output_to_file(source_directory, subdirectory, baseline, best_run_time, best_configuration, worst_run_time, worst_configuration):

	outfile = os.path.join(source_directory, subdirectory, "compilation_report.txt")
	file = open(outfile, 'w')
	file.write("-------------------BASELINE CONFIG---------------------")
	file.write("\n")
	file.write("Time: ")
	file.write(str(baseline))
	file.write("\n\n\n\n")		
	file.write("-------------------BEST CONFIG---------------------")
	file.write("\n")
	file.write("Time: ")
	file.write(str(best_run_time))
	file.write("\n")
	file.write("Config: ")
	file.write(best_configuration)
	file.write("\n\n\n\n")
	file.write("-------------------WROST CONFIG---------------------")
	file.write("\n")
	file.write("Time: ")
	file.write(str(worst_run_time))
	file.write("\n")
	file.write("Config: ")
	file.write(worst_configuration)



if __name__ == '__main__':
	main()
