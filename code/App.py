
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

## This is a list of all the programs in beebs
subdirectories = [ name for name in os.listdir(config.beebs_directory)
		if os.path.isdir(os.path.join(config.beebs_directory, name)) ]

## Parse the Flags into a list
flags = FlagParser.main(config.database_path)

def main():

	################################### Initial directory structure setup ###################################
	## Build the directory for the optimistation level flag
	build_directory("optimisation-level", config.optimisation_settings)
	for flag in flags:
		## Build the directory for every other flag
		directory_name = "dir" + flag
		build_directory(directory_name, config.binary_settings)

	################################### And this is the pipeline ###################################
	if(config.run_iterative_compilation):
		## Iterative Compilation
		report = IterativeCompiler.main(config.beebs_directory, config.database_path, config.iterative_compilation_depth, config.output_directory)


	## IR Parse
	for subdirectory in subdirectories:

		## SSA generation
		generated = IntermediateRepresentationGenerator.main(config.beebs_directory, subdirectory)

		## If SSA generation worked
		if(generated):
			## Fetch the configuration
			#compiler_configuration = CRReader.binary_flag_reader(config.beebs_directory, subdirectory)
			## And parse the optimisation flag
			optimisation_level = CRReader.optimisation_flag_reader(config.output_directory, subdirectory)

			if(not optimisation_level == False):
			#or not compiler_configuration == False):


				## And write accordingly to that datum
				directory_name = "optimisation-level"
				filepath = os.path.join(config.output_directory, "training-data", directory_name, "train.kb")
				file = open(filepath, 'a')
				file.write(IntermediateRepresentationParser.main(config.beebs_directory, subdirectory, optimisation_level))		

				#for flag in flags:
				#	directory_name = "dir" + flag
				#	filepath = os.path.join(config.training_data_directory, directory_name, "train.kb")
				#	file = open(filepath, 'a')
					
					## Parse the intermediate representation once per flag.
				#	if flag in compiler_configuration:
				#		file.write(IntermediateRepresentationParser.main(config.beebs_directory, subdirectory, True))
				#	else:
				#		file.write(IntermediateRepresentationParser.main(config.beebs_directory, subdirectory, False))

				file.close()
		else:
			print "==========", subdirectory, "==========" 
	
	################################### Model Builder ###################################
	build_all()

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

def build_model(path):
	print subprocess.Popen("chmod +x " + os.path.join(path, "run.sh"), shell=True, stdout=subprocess.PIPE).stdout.read()
	print subprocess.Popen(os.path.join(path, "run.sh"), shell=True, stdout=subprocess.PIPE).stdout.read()

def build_all():

	training_data_directory = os.path.join(config.output_directory, "training-data")
	if not os.path.exists(training_data_directory):
		os.makedirs(training_data_directory)

	path = os.path.join(training_data_directory, "optimisation-level")
	if not os.path.exists(path):
		os.makedirs(path)
	build_model(path)

	#for flag in flags:
	#	path = os.path.join(training_data_directory, "dir" + flag)
	#	if not os.path.exists(path):
	#		os.makedirs(path)
	#	build_model(path)


if __name__ == '__main__':
	main()