
import config
import parser.IntermediateRepresentationGenerator as IntermediateRepresentationGenerator
import parser.IntermediateRepresentationParser as IntermediateRepresentationParser
import subprocess
import os
import iterative_compiler.command_executor as command_executor
import iterative_compiler.command_generator as command_generator

flags = ["-fipa-pta", "-ftree-loop-if-convert-stores", "-ftree-loop-im", "-ftree-loop-ivcanon", "-ftree-vectorize", "-fstrict-aliasing", "-fbtr-bb-exclusive", "-ffast-math", "-funroll-loops"]


def classify(subdirectories):

	runtimes = {}

	for subdirectory in subdirectories:
		IntermediateRepresentationGenerator.main(config.beebs_directory, subdirectory)
		ace_instance = IntermediateRepresentationParser.main(config.beebs_directory, subdirectory, "%%%%%%%%%%")


		output,error = subprocess.Popen( "echo 'prog.' | swipl -s " + os.path.join(config.output_directory, "training-data", "optimisation-level", "test.P"), shell=True, stdout=subprocess.PIPE).communicate()
		file = open("/home/john/Thesis/output/training-data/optimisation-level/predicted.txt")
		predicted_class = file.read().replace(" ", "").replace("\n", "")

		if predicted_class == "two":
			configuration = ["-O2"]
		else:
			configuration = ["-O3"]

		for flag in flags:
			output,error = subprocess.Popen( "echo 'prog.' | swipl -s " + os.path.join(config.output_directory, "training-data", "dir" + flag, "test.P"), shell=True, stdout=subprocess.PIPE).communicate()
			file = open("/home/john/Thesis/output/training-data/dir" + flag + "/predicted.txt")
			predicted_class = file.read().replace(" ", "").replace("\n", "")
			if(predicted_class == "on"):
				configuration.append(flag)

		compile_command = command_generator.parse_configuration(configuration, os.path.join(config.beebs_directory, subdirectory))
		run_command = command_generator.generate_run_command(os.path.join(config.beebs_directory, subdirectory), subdirectory)

		print compile_command
		print run_command
		
		if( command_executor.compile_program(compile_command) ):
			runtimes[subdirectory] = command_executor.run_program(run_command, config.output_directory, subdirectory)


	return runtimes

def classify_flag(subdirectories):

	runtimes = {}

	for subdirectory in subdirectories:
		IntermediateRepresentationGenerator.main(config.beebs_directory, subdirectory)
		ace_instance = IntermediateRepresentationParser.main(config.beebs_directory, subdirectory, "%%%%%%%%%%")


		output,error = subprocess.Popen( "echo 'prog.' | swipl -s " + os.path.join(config.output_directory, "training-data", "optimisation-level", "test.P"), shell=True, stdout=subprocess.PIPE).communicate()
		file = open("/home/john/Thesis/output/training-data/optimisation-level/predicted.txt")
		return file.read().replace(" ", "").replace("\n", "")


if __name__ == "__main__":
	classify("rijndael")