
#!/usr/bin/env python

'''
This is the pipeline for this prject. This runs everything from iterative compilation to model building.
Should take days to run so use with caution!!!
'''


import os
import subprocess
import config
import IterativeCompiler
import IntermediateRepresentationGenerator
import IntermediateRepresentationParser
import FlagParser
import CRReader

## This is a list of all the programs in beebs
subdirectories = [ name for name in os.listdir(config.source_directory) if os.path.isdir(os.path.join(config.source_directory, name)) ]


def main():

	## Parse the Flags into a list
	flags = FlagParser.main(config.flag_database)
	for flag in flags:

		directory = os.path.join(config.training_data_directory, "dir" + flag)
		if not os.path.exists(directory):
			os.makedirs(directory)

		filepath = os.path.join(config.training_data_directory, "dir" + flag, "train.kb")
		file = open(filepath, 'w')
		file.close()

	## And this is the pipeline
	for subdirectory in subdirectories:
		if(config.run_iterative_compilation):
			## Run Iterative Compilation (and generate the compilation reports).
			## A nice addition would be a supplied variable as to whether you want to run
			## iterative compilation again, or proceed with previously generated compilation reports.
			IterativeCompiler.main(config.source_directory, subdirectory, config.flag_database, config.iterative_compilation_depth)

		## Change the c files into the ssa representation.
		generated = IntermediateRepresentationGenerator.main(config.source_directory, subdirectory)

		if(generated):
			compiler_configuration = CRReader.main(config.source_directory, subdirectory)
			for flag in flags:

				#print flag

				filepath = os.path.join(config.training_data_directory, "dir" + flag, "train.kb")
				file = open(filepath, 'a')
				
				## Parse the intermediate representation once per flag.
				if flag in compiler_configuration:
					file.write(IntermediateRepresentationParser.main(config.source_directory, subdirectory, flag, True))
				else:
					file.write(IntermediateRepresentationParser.main(config.source_directory, subdirectory, flag, False))

				file.close()

	print subprocess.Popen("rm -rf" + config.training_data_directory, shell=True, stdout=subprocess.PIPE).stdout.read()


	for directory in os.walk(config.training_data_directory):
		path = str(directory[0]) + "/" + "train.s"
		settings_file = open(path, 'w')
		settings_file.write("""

load(models).
prune_rules(false).
use_packs(ilp).

classes([true, false]).


use_packs(0).
resume(off).
sampling_strategy(none).

warmode(method(+-obj)).
warmode(basic_block(+-obj)).
warmode(in(+-obj, +-obj)).
warmode(directed_edge(+-obj, +-obj)).
warmode(method_call(+-obj, +-obj)).

load_package(query).
log_queries([prettypacks]).

minfreq(0.5).
warmr_maxdepth(4).
warmode(method_call(+-obj, +-obj)).


""")
		settings_file.close()

	
	for directory in os.walk(config.training_data_directory):
		path = str(directory[0]) + "/" + "run.sh"
		print "-------------------------------------------"
		print path
		print "-------------------------------------------"
		runner_script = open(path, 'w')
		runner_script.write("""

#!/usr/bin/env bash
 
SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
echo "$SCRIPT_DIR"
cd $SCRIPT_DIR

echo "tilde" | $ACE_ILP_ROOT/bin/ace
echo "warmr" | $ACE_ILP_ROOT/bin/ace


""")
		runner_script.close()


		print subprocess.Popen("chmod +x " + path, shell=True, stdout=subprocess.PIPE).stdout.read()
		print subprocess.Popen(path, shell=True, stdout=subprocess.PIPE).stdout.read()



if __name__ == '__main__':
	main()