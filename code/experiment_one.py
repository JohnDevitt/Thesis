
#!/usr/bin/env python

'''
This is the pipeline for this prject. This runs everything from iterative compilation to model building.
Should take days to run so use with caution!!!
'''

from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.tree import DecisionTreeClassifier
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

	initial_setup()

	actualResults = []
	decisionTreeResults = []
	randomForestResults = []
	one_nnResults = []
	five_nnResults = []
	relational_results = []

	initial_setup()

	for test_dir in subdirectories:
		
		test_features = read_features(test_dir)
		
		if test_features:
			test_set = [test_dir]
			training_set = [subdirectory for subdirectory in subdirectories if subdirectory not in test_set]

			features_list = {}
			configuration_list = {}

			initial_setup()

			for subdirectory in training_set:

				features = read_features(subdirectory)
				if features:
					
					features_list[subdirectory] = features
					configuration_list[subdirectory] = CRReader.optimisation_flag_reader(config.output_directory, subdirectory)


					ace_instance = IntermediateRepresentationParser.main(config.beebs_directory, subdirectory, configuration_list[subdirectory])
					write_training_data_to_file("optimisation-level", config.output_directory, ace_instance)


			list_values = [ v for v in features_list.values() ]
			X = numpy.array(list_values)

			list_values = [ v for v in configuration_list.values() ]
			y = numpy.array(list_values)

			decisionTree = DecisionTreeClassifier()
			randomForest = RandomForestClassifier()
			one_nn = KNeighborsClassifier(n_neighbors=1)
			five_nn = KNeighborsClassifier()

			decisionTree.fit(X, y)
			randomForest.fit(X, y)
			one_nn.fit(X, y)
			five_nn.fit(X, y)
			build_all()

			actualResults.append(CRReader.optimisation_flag_reader(config.output_directory, test_dir))
			decisionTreeResults.append(decisionTree.predict(test_features)[0])
			randomForestResults.append(randomForest.predict(test_features)[0])
			one_nnResults.append(one_nn.predict(test_features)[0])
			five_nnResults.append(five_nn.predict(test_features)[0])
			relational_results.append(relational_classifier.classify_flag(test_dir))


	decisionAccuracy = 0
	forestsAccuracy = 0
	one_nnAccuracy = 0
	five_nnAccuracy = 0
	tildeAccuracy = 0


	for i in range(0, len(actualResults)):
		if(actualResults[i] == decisionTreeResults[i]):
			decisionAccuracy = decisionAccuracy + 1

		if(actualResults[i] == randomForestResults[i]):
			forestsAccuracy = forestsAccuracy + 1

		if(actualResults[i] == one_nnResults[i]):
			one_nnAccuracy = one_nnAccuracy + 1

		if(actualResults[i] == five_nnResults[i]):
			five_nnAccuracy = five_nnAccuracy + 1

		if(actualResults[i] == relational_results[i]):
			tildeAccuracy = tildeAccuracy + 1



	print float(decisionAccuracy)/float(len(actualResults))
	print float(forestsAccuracy)/float(len(actualResults))
	print float(one_nnAccuracy)/float(len(actualResults))
	print float(five_nnAccuracy)/float(len(actualResults))
	print float(tildeAccuracy)/float(len(actualResults))






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