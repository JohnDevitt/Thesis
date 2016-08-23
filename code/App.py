
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

## This is a list of all the programs in beebs
subdirectories = [ name for name in os.listdir(config.beebs_directory)
		if os.path.isdir(os.path.join(config.beebs_directory, name)) ]

## Parse the Flags into a list
flags = FlagParser.main(config.database_path)

def main():

	total = 0
	correct = 0

	## Iterative Compilation
	if(config.run_iterative_compilation):
		report = IterativeCompiler.main(config.beebs_directory, config.database_path, config.iterative_compilation_depth, config.output_directory)

	## Build the ACE directory
	build_directory("optimisation-level", config.optimisation_settings)

	## Leave on out cv
	for target_data in subdirectories:

		## Reset the data to be mined
		reset_knowledge_base("optimisation-level")

		gen = [subdirectory for subdirectory in subdirectories if subdirectory != target_data and subdirectory != "blowfish" and subdirectory != "trio"]
		for subdirectory in gen:

			## Build the mine
			IntermediateRepresentationGenerator.main(config.beebs_directory, subdirectory) 
			instance_class = CRReader.optimisation_flag_reader(config.output_directory, subdirectory)
			ace_instance = IntermediateRepresentationParser.main(config.beebs_directory, subdirectory, instance_class)
			if(not instance_class == False):
				write_training_data_to_file("optimisation-level", config.output_directory, ace_instance)

		## Build the model
		build_all()
		## Generate target data
		IntermediateRepresentationGenerator.main(config.beebs_directory, target_data)
		target_class = CRReader.optimisation_flag_reader(config.output_directory, target_data)
		ace_target = IntermediateRepresentationParser.main(config.beebs_directory, target_data, "%%%%%%")
		## Write to file
		write_testing_data_to_file("optimisation-level", config.output_directory, ace_target)
		write_model_to_file("optimisation-level", config.output_directory)
		## Run the model on the data
		output,error = subprocess.Popen( "echo 'prog.' | swipl -s " + os.path.join(config.output_directory, "training-data", "optimisation-level", "test.P"), shell=True, stdout=subprocess.PIPE).communicate()
		## Get results
		file = open("/home/john/Thesis/output/training-data/optimisation-level/predicted.txt")
		predicted_class = file.read().replace(" ", "").replace("\n", "")


		if predicted_class == target_class:
			correct = correct + 1
		else:
			file = open("/home/john/Thesis/output/training-data/optimisation-level/failed.txt", 'a')
			file.write(target_data + "\n")
			file.close()

		total = total + 1

		file = open("/home/john/Thesis/output/training-data/optimisation-level/results.txt", 'w')
		file.write("total: " + str(total) + "\n")
		file.write("correct: " + str(correct) + "\n")
		file.write("accuracy:" + str((correct*100)/total) + "% \n")
		file.close()

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
	file = open(filepath, 'a')
	file.write(model)

	file.write('''prog :- 
	open("/home/john/Thesis/output/training-data/optimisation-level/predicted.txt", write, Stream),
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



if __name__ == '__main__':
	main()