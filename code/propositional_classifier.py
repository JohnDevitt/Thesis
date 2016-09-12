
import numpy
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import RandomForestClassifier
import os
import iterative_compiler.command_executor as command_executor
import iterative_compiler.command_generator as command_generator
import config

flags = ["-fipa-pta", "-ftree-loop-if-convert-stores", "-ftree-loop-im", "-ftree-loop-ivcanon", "-ftree-vectorize", "-fstrict-aliasing", "-fbtr-bb-exclusive", "-ffast-math", "-funroll-loops"]

def build_classifiers(features_list, configuration_list):


	if not configuration_list:
		list_values = []
	else:
		list_values = [ v for v in features_list.values() ]
	X = numpy.array(list_values)

	randomForests = {}
	randomForests["optimisation_level"] = RandomForestClassifier()

	nearestNeighbours = {}
	nearestNeighbours["optimisation_level"] = KNeighborsClassifier()

	y = []
	for subdirectory in configuration_list:
		if not configuration_list:
			y.append(configuration_list[subdirectory])
		else:
			y.append(configuration_list[subdirectory][0])

	randomForests["optimisation_level"].fit(X, numpy.array(y))
	nearestNeighbours["optimisation_level"].fit(X, numpy.array(y))

	if configuration_list:
		for flag in flags:
			y = []
			for subdirectory in configuration_list:
				if flag in configuration_list[subdirectory]:
					y.append("on")
				else:
					y.append("off")

			randomForests[flag] = RandomForestClassifier()
			randomForests[flag].fit(X, numpy.array(y))

			nearestNeighbours[flag] = KNeighborsClassifier()
			nearestNeighbours[flag].fit(X, numpy.array(y))

	return randomForests, nearestNeighbours

def classify(test_set, classifiers):

	configurations = {}
	for instance in test_set:
		features = read_features(instance)
		if classifiers["optimisation_level"].predict(features) == "two":
			configuration = ["-O2"]
		else:
			configuration = ["-O3"]
		for flag in flags:
			if classifiers[flag].predict(features) == "on":
				configuration.append(flag)

		configurations[instance] = configuration

	runtimes = {}

	for subdirectory in configurations:
		compile_command = command_generator.parse_configuration(configurations[subdirectory], os.path.join(config.beebs_directory, subdirectory))
		run_command = command_generator.generate_run_command(os.path.join(config.beebs_directory, subdirectory), subdirectory)
	
		if( command_executor.compile_program(compile_command) ):
			runtimes[subdirectory] = command_executor.run_program(run_command, config.output_directory, subdirectory)


	return runtimes

def classify_flag(features, classifiers):

	print features

	print "=================="
	print classifiers["optimisation_level"].predict(features)
	print "=================="
	return classifiers["optimisation_level"].predict(features)


def read_features(directory):
	filepath = os.path.join('/home/john/Desktop/cbeebs-ft-extract/src', directory, 'ici_features_function.ft')
	features_file = open(filepath, 'rb')
	features = features_file.read().split()
	features = [feature[2:].split('=') for feature in features]
	features = sorted([[ float(feature[0]), float(feature[1].replace(",", "")) ] for feature in features])
	features = [ feature[1] for feature in features ]
	features_file.close()

	return features