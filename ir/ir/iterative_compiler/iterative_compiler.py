
#!/usr/bin/env python

import configuration_generator
import command_generator
import command_executor
import compilation_report_generator
import copy as cp
import os
import numpy


def main(directory, filename, flag_database_location, iterative_compilation_depth):

	## Initial Setup
	flags = configuration_generator.generate_flags(flag_database_location)
	configurations = configuration_generator.generate_configurations(flags, iterative_compilation_depth)
	baseline_runtime = generate_baseline_runtime(directory, filename)

	## Initial Loop
	initial_runtimes = generate_initial_runtimes(directory, filename, configurations)
	initial_best = min(initial_runtimes)
	initial_configuration = configurations[initial_runtimes.index(initial_best)]

	## Optimisations, Step 1:
	optimised_runtimes = find_bad_flags(directory, filename, initial_best, initial_configuration)
	optimised_configuration = remove_flags(initial_best, initial_configuration, optimised_runtimes)
	optimised_best = min(initial_runtimes + optimised_runtimes)

	## Optimisations, Step 2:
	best_runtimes = find_good_flags(directory, filename, optimised_best, optimised_configuration, flags)
	best_configuration = add_flags(optimised_best, optimised_configuration, best_runtimes, flags)
	best = min(initial_runtimes + optimised_runtimes + best_runtimes)

	compilation_report_generator.generate_report(baseline_runtime, best_configuration, best, directory, best_runtimes)


def generate_baseline_runtime(directory, filename):

	return compile_and_run(directory, filename, ['-O3'])

def generate_initial_runtimes(directory, filename, configurations):

	return [compile_and_run(directory, filename, configuration) for configuration in configurations]

def find_bad_flags(directory, filename, baseline_runtime, configuration):

	working_configuration = cp.deepcopy(configuration)
	runtimes = []

	for flag in configuration:
		tmp_configuration = cp.deepcopy(working_configuration)
		tmp_configuration.remove(flag)

		runtime = compile_and_run(directory, filename, tmp_configuration)
		runtimes.append(runtime)

		if(runtime < baseline_runtime):
			baseline_runtime = runtime
			working_configuration.remove(flag)

	return runtimes

def find_good_flags(directory, filename, baseline_runtime, configuration, flags):


	working_configuration = cp.deepcopy(configuration)
	runtimes = []

	alternate_flags = [flag for flag in flags[get_optimisiation_level(configuration)]
			if flag not in configuration]

	for flag in alternate_flags:
		tmp_configuration = cp.deepcopy(working_configuration)
		tmp_configuration.append(flag)

		runtime = compile_and_run(directory, filename, tmp_configuration)
		runtimes.append(runtime)

		if(runtime < baseline_runtime):
			baseline_runtime = runtime
			working_configuration = cp.deepcopy(tmp_configuration)

	return runtimes

def remove_flags(best_runtime, configuration, runtimes):

	working_best = best_runtime
	working_configuration = cp.deepcopy(configuration)

	for flag, runtime in zip(configuration, runtimes)[1:]:
		if(runtime < working_best):
			working_best = runtime
			working_configuration.remove(flag)

	return working_configuration

	# return [flag for runtime, flag in zip(runtimes, configuration) if runtime > best_runtime]

def add_flags(best_runtime, configuration, runtimes, flags):

	alternate_flags = [flag for flag in flags[get_optimisiation_level(configuration)]
			if flag not in configuration]

	working_best = best_runtime
	working_configuration = cp.deepcopy(configuration)

	for flag, runtime in zip(alternate_flags, runtimes)[1:]:
		if(runtime < working_best):
			working_best = runtime
			working_configuration.append(flag)

	return working_configuration

def compile_and_run(directory, filename, configuration):

	## Generate Bash Commands
	run_command = command_generator.generate_run_command(directory, filename)
	compile_command = command_generator.parse_configuration(configuration, directory)

	## Run Bash Commands
	if( command_executor.compile_program(compile_command) ):
		return command_executor.run_program(run_command)

	return 0

def get_optimisiation_level(configuration):
	return configuration[0][1:]