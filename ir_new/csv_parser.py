#!/usr/bin/env python


import os
import csv
import random
import subprocess
import time

def main():



	source_directory = "/home/john/Thesis/beebs/src"
	flag_databse = "arm-none-eabi-gcc-4.8.2-flags.csv"
	
	subdirectories = [ name for name in os.listdir(source_directory) if os.path.isdir(os.path.join(source_directory, name)) ]
	optimisation_flags = build_flag_lists(flag_databse)


	for subdirectory in subdirectories:

		best_run_time = float("inf")
		best_configuration = ""

		worst_run_time = 0
		worst_configuration = ""


		for i in range(1, 3):

			compile_command = build_compile_command(subdirectory, optimisation_flags)
			make_system_call(compile_command)

			run_command = build_run_command(subdirectory)
			run_time = timed_make_system_call(run_command)

			if(run_time < best_run_time):
				best_run_time= run_time
				best_configuration = compile_command

			if(run_time > worst_run_time):
				worst_run_time= run_time
				worst_configuration = compile_command


def build_flag_lists(filepath):
	optimisation_flags = []

	with open(filepath, 'rb') as file:
		reader = csv.reader(file)

		for i in range(2, 5):
			optimisation_flags.append([row[0] for row in reader if row[i] == '1'])
			file.seek(0)

	return optimisation_flags

def build_compile_command(subdirectory, optimisation_flags):
	compile_command = "make -C " + subdirectory + "/ clean; make -C " + subdirectory + "/ CFLAGS='"
	optimisation_level = random.randint(1, 3)

	compile_command += "-O"
	compile_command += str(optimisation_level)
	compile_command += " "

	for flag in optimisation_flags[optimisation_level - 1]:
		if(random.uniform(0, 1) < 0.1):
			compile_command += flag
			compile_command += " "

	compile_command += "'"

	return compile_command

def build_run_command(subdirectory):
	return subdirectory + "/" + subdirectory

def make_system_call(command):
	print subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.read()

def timed_make_system_call(command):
	start = time.time()
	make_system_call(command)
	end = time.time()

	return end - start



if __name__ == '__main__':
	main()
