
#!/usr/bin/env python

import configuration_generator_2
import command_generator
import command_executor
import compilation_report_generator
import copy as cp
import os
import numpy


def main(directory, filename, flag_database_location, iterative_compilation_depth, output_directory):

	flags = ["-fipa-pta", "-ftree-loop-if-convert-stores", "-ftree-loop-im", "-ftree-loop-ivcanon", "-ftree-vectorize", "-fstrict-aliasing", "-fbtr-bb-exclusive", "-ffast-math", "-funroll-loops"]

	best_runtime = float('inf')
	best_configuration = []

	baseline_runtime = generate_baseline_runtime(directory, filename, output_directory)
	configurations = configuration_generator_2.generate_configurations()

	for configuration in configurations:
		runtime = compile_and_run(directory, filename, configuration, output_directory)
		if runtime < best_runtime:
			best_runtime = runtime
			best_configuration = configuration

	for flag in flags:
		current_configuration = cp.deepcopy(best_configuration)
		if flag in best_configuration:
			current_configuration.remove(flag)
		else:
			current_configuration.append(flag)

		runtime = compile_and_run(directory, filename, configuration, output_directory):
		if runtime < best_runtime:
			best_runtime = runtime
			best_configuration = cp.deepcopy(current_configuration)

	compilation_report_generator_2.generate_report(baseline_runtime, best_configuration, best_runtime, filename output_directory)


def generate_baseline_runtime(directory, filename, output_directory):
	return compile_and_run(directory, filename, ['-O3'], output_directory)

def compile_and_run(directory, filename, configuration, output_directory):

	## Generate Bash Commands
	compile_command = command_generator.parse_configuration(configuration, directory)
	run_command = command_generator.generate_run_command(directory, filename)

	## Run Bash Commands
	if( command_executor.compile_program(compile_command) ):
		return command_executor.run_program(run_command, output_directory, filename)

	return float('inf')