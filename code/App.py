
#!/usr/bin/env python

'''
This is the pipeline for this prject. This runs everything from iterative compilation to model building.
Should take days to run so use with caution!!!
'''


import os
import subprocess
import config
import iterative_compiler.App as IterativeCompiler
import parser.IntermediateRepresentationGenerator as IntermediateRepresentationGenerator
import parser.IntermediateRepresentationParser as IntermediateRepresentationParser
import parser.FlagParser as FlagParser
import parser.CRReader as CRReader
import ModelExtractor
import random
import relational_classifier
import propositional_classifier
import numpy
import iterative_compiler.command_executor as command_executor
import iterative_compiler.command_generator as command_generator

## This is a list of all the programs in beebs
subdirectories = [ name for name in os.listdir(config.beebs_directory)
		if os.path.isdir(os.path.join(config.beebs_directory, name)) ]

flags = ["-fipa-pta", "-ftree-loop-if-convert-stores", "-ftree-loop-im", "-ftree-loop-ivcanon", "-ftree-vectorize", "-fstrict-aliasing", "-fbtr-bb-exclusive", "-ffast-math", "-funroll-loops"]

def main():

	test_set = random.sample(subdirectories, len(subdirectories)/5)
	training_set = [subdirectory for subdirectory in subdirectories if subdirectory not in test_set]

	features_list = {}
	configuration_list = {}

	initial_setup()

	for subdirectory in training_set:

		## Instance
		IntermediateRepresentationGenerator.main(config.beebs_directory, subdirectory) 

		## Target Variables
		optimisation_level = CRReader.optimisation_flag_reader(config.output_directory, subdirectory)
		optimisation_flags = CRReader.binary_flag_reader(config.output_directory, subdirectory)

		features = read_features(subdirectory)
		if features:
			features_list[subdirectory] = features
			configuration_list[subdirectory] = [optimisation_level]
			
			ace_instance = IntermediateRepresentationParser.main(config.beebs_directory, subdirectory, optimisation_level)
			write_training_data_to_file("optimisation-level", config.output_directory, ace_instance)
			
			for flag in flags:
				if flag in optimisation_flags:
					configuration_list[subdirectory].append(flag)
					ace_instance = IntermediateRepresentationParser.main(config.beebs_directory, subdirectory, "on")
				else:
					ace_instance = IntermediateRepresentationParser.main(config.beebs_directory, subdirectory, "off")
				
				write_training_data_to_file("dir" + flag, config.output_directory, ace_instance)

	test_set = [instance for instance in test_set if read_features(instance)]


	baselines = {}
	for subdirectory in test_set:
		compile_command = command_generator.parse_configuration(["-O3"], os.path.join(config.beebs_directory, subdirectory))
		run_command = command_generator.generate_run_command(os.path.join(config.beebs_directory, subdirectory), subdirectory)
	
		if( command_executor.compile_program(compile_command) ):
			baselines[subdirectory] = command_executor.run_program(run_command, config.output_directory, subdirectory)




	randomForests, nearestNeighbours = propositional_classifier.build_classifiers(features_list, configuration_list)
	forestResults = propositional_classifier.classify(test_set, randomForests)
	neighoursResults = propositional_classifier.classify(test_set, nearestNeighbours)

	build_all()
	relational_results = relational_classifier.classify(test_set)

	print " "
	print baselines
	print " "
	print forestResults
	print " "
	print neighoursResults
	print " "
	print relational_results

	return baselines, forestResults, neighoursResults, relational_results



def initial_setup():
	## Iterative Compilation
	if(config.run_iterative_compilation):
		IterativeCompiler.main(config.beebs_directory, config.database_path, config.iterative_compilation_depth, config.output_directory)

	## Build directories
	build_directory("optimisation-level", config.optimisation_settings)
	for flag in flags:
		build_directory("dir" + flag, config.binary_settings)

def write_training_data_to_file(directory_name, output_directory ,instance_data):
	filepath = os.path.join(config.output_directory, "training-data", directory_name, "train.kb")
	file = open(filepath, 'a')
	file.write(instance_data)		
	file.close()

def write_testing_data_to_file(directory_name, output_directory, target_data):
	filepath = os.path.join(config.output_directory, "training-data", directory_name, "test.P")
	file = open(filepath, 'w')
	file.write(target_data)		
	file.close()

def write_model_to_file(directory_name, output_directory):
	model = ModelExtractor.extractModel(config.output_directory, directory_name)
	filepath = os.path.join(config.output_directory, "training-data", directory_name, "test.P")
	file = open(filepath, 'w')
	file.write(model)

	file.write('''prog :- 
	open("/home/john/Thesis/output/training-data/''' + directory_name + '''/predicted.txt", write, Stream),
	(class([A]), write(Stream, '\n'), write(Stream, A), write(Stream, '\n'), fail
	; true
	),
	close(Stream).\n\n\n''')

	file.close()

def reset_knowledge_base(directory_name):

	directory = os.path.join(config.output_directory, "training-data" ,directory_name)

	## Build the knowledge base
	filepath = os.path.join(directory, "train.kb")
	file = open(filepath, 'w')
	file.close()

def build_directory(directory_name, settings):

	## Build the directory
	directory = os.path.join(config.output_directory, "training-data" ,directory_name)
	if not os.path.exists(directory):
		os.makedirs(directory)

	## Build the knowledge base
	filepath = os.path.join(directory, "train.kb")
	file = open(filepath, 'w')
	file.close()

	## Build the settings file
	path = os.path.join(directory, "train.s")
	settings_file = open(path, 'w')
	settings_file.write(settings)
	settings_file.close()

	## Build the runner script
	path = os.path.join(directory, "run.sh")
	runner_script = open(path, 'w')
	runner_script.write(config.runner_script)
	runner_script.close()

	## Build the runner script
	path = os.path.join(directory, "predicted.txt")
	runner_script = open(path, 'w')
	runner_script.close()

	filepath = os.path.join(directory, "failed.txt")
	file = open(filepath, 'w')
	file.close()

def build_model(path):
	subprocess.call("chmod +x " + os.path.join(path, "run.sh"), shell=True, stdout=subprocess.PIPE)
	subprocess.call(os.path.join(path, "run.sh"), shell=True, stdout=subprocess.PIPE)

def build_all():

	training_data_directory = os.path.join(config.output_directory, "training-data")
	if not os.path.exists(training_data_directory):
		os.makedirs(training_data_directory)

	path = os.path.join(training_data_directory, "optimisation-level")
	if not os.path.exists(path):
		os.makedirs(path)
	build_model(path)

	for flag in flags:
		path = os.path.join(training_data_directory, "dir" + flag)
		if not os.path.exists(path):
			os.makedirs(path)
		build_model(path)

	write_model_to_file("optimisation-level", config.output_directory)
	for flag in flags:
		write_model_to_file("dir" + flag, config.output_directory)

def read_features(directory):
	filepath = os.path.join('/home/john/Desktop/cbeebs-ft-extract/src', directory, 'ici_features_function.ft')
	features_file = open(filepath, 'rb')
	features = features_file.read().split()
	features = [feature[2:].split('=') for feature in features]
	features = sorted([[ float(feature[0]), float(feature[1].replace(",", "")) ] for feature in features])
	features = [ feature[1] for feature in features ]
	features_file.close()

	return features


if __name__ == '__main__':
	main()