
#!/usr/bin/env python

import configuration_generator
import command_generator
import command_executor
import compilation_report_generator
import copy as cp
import os

def main(directory, filename, flag_database_location, iterative_compilation_depth):

	## Load the data from the database into memory
	optimisation_specific_flags = configuration_generator.generate_optimistaion_specific_flags(flag_database_location)
	configuration_list = configuration_generator.generate_configuration_list(optimisation_specific_flags, iterative_compilation_depth)

	## Print configuration_list
	baseline_runtime = build_baseline(directory, filename)
	print "Running Iterative Compilation For: " + directory
	best_runtime, best_configuration = iteratively_compile(directory, filename, configuration_list)
	print "Removing Flags For: " + directory
	best_runtime, best_configuration = remove_present_flags(directory, filename, best_configuration, optimisation_specific_flags)
	print "Adding Flags For: " + directory
	best_runtime, best_configuration = add_missing_flags(directory, filename, best_configuration, optimisation_specific_flags)
	return compilation_report_generator.main(baseline_runtime, best_configuration, best_runtime, directory)

def build_baseline(directory, filename):
	
	run_command = generate_run_command(directory, filename)
	compile_command = command_generator.generate_compile_command('-O3', directory)
	successful, runtime = command_executor.run_program(compile_command, run_command)
	
	print runtime
	return runtime

def iteratively_compile(directory, filename, configuration_list):

	runtime_total = []

	## Set up initial best
	best_runtime = float('inf')
	best_configuration = []

	## For each configuration
	for configuration in configuration_list:
		
		#print configuration

		## Generate the bash commands
		run_command = command_generator.generate_run_command(directory, filename)
		compile_command = command_generator.parse_configuration(configuration, directory)
		
		## Run the commands and collect the results
		successful, runtime = compile_and_run(directory, filename, configuration)
		runtime_total.append(runtime)

		## If the results are good update the data
		if(runtime < best_runtime):
			best_runtime = runtime
			best_configuration = configuration

		print best_runtime
	## Return the best ones either way.
	return best_runtime, best_configuration

def remove_present_flags(directory, filename, configuration, optimisation_specific_flags):

	runtime_total = []

	## Set up initial best
	successful, best_runtime = compile_and_run(directory, filename, configuration)
	best_configuration = []

	## For each flag in the current configuration
	for flag in configuration[1: len(configuration)]:
				
		## Create a copy of the configuration without that flag
		tmp_configuration = cp.deepcopy(configuration)
		tmp_configuration.remove(flag)

		#print tmp_configuration

		## Generate the bash commands
		run_command = command_generator.generate_run_command(directory, filename)
		compile_command = command_generator.parse_configuration(tmp_configuration, directory)
		
		## Run the commands and collect the results
		successful, runtime = compile_and_run(directory, filename, tmp_configuration)
		runtime_total.append(runtime)

		## If the results are good update the data
		if(runtime < best_runtime):
			best_runtime = runtime
			best_configuration = tmp_configuration

		print best_runtime
	## Return the best ones either way.
	return best_runtime, best_configuration

def add_missing_flags(directory, filename, configuration, optimisation_specific_flags):

	runtime_total = []

	## Set up initial best
	successful, best_runtime = compile_and_run(directory, filename, configuration)
	best_configuration = []

	## For each flag in the current configuration
	alt_flags = [flag for flag in (optimisation_specific_flags[configuration[0][1:]]) if flag not in configuration]
	for flag in alt_flags:

		## Create a copy of the configuration without that flag
		tmp_configuration = cp.deepcopy(configuration)
		tmp_configuration.append(flag)

		#print tmp_configuration
		## Generate the bash commands
		run_command = command_generator.generate_run_command(directory, filename)
		compile_command = command_generator.parse_configuration(tmp_configuration, directory)
		
		## Run the commands and collect the results
		successful, runtime = compile_and_run(directory, filename, tmp_configuration)
		runtime_total.append(runtime)

		## If the results are good update the data
		if(runtime < best_runtime):
			best_runtime = runtime
			best_configuration = tmp_configuration
	
		print best_runtime
	## Return the best ones either way.
	return best_runtime, best_configuration


def compile_and_run(directory, filename, configuration):

	## Generate Bash Commands
	run_command = command_generator.generate_run_command(directory, filename)
	compile_command = command_generator.parse_configuration(configuration, directory)

	## Run Bash Commands
	command_executor.compile_program(compile_command)
	return command_executor.run_program(run_command)
