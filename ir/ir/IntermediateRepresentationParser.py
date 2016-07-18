#!/usr/bin/env python

import re
import ControlFlowAnalyser
import MethodSeperator
import FlagParser
import os


def main(source_directory, subdirectory):



	## FILE IO
	filepath = os.path.join(source_directory, subdirectory) + "/" + subdirectory + ".c.018t.ssa"
	if os.path.exists(filepath):
		raw_ir = open(filepath, "r")
		processed_ir = raw_ir.readlines()
		good_ace_ir = "\nbegin(model(" + subdirectory + "eff" + ")).\n"
		good_ace_ir += "efficient.\n"

		bad_ace_ir = "\nbegin(model(" + subdirectory + "ineff" + ")).\n"
		bad_ace_ir += "inefficient.\n"



		## First, break the code down into it's seperate methods.
		method_dict = MethodSeperator.build_method_dict(processed_ir)



###############################################################################################

		good_config, bad_config = FlagParser.main(source_directory, subdirectory)
		good_ace_ir += good_config
		good_ace_ir += "\n\n\n\n"

		bad_ace_ir += bad_config
		bad_ace_ir += "\n\n\n\n"



###############################################################################################

		good_ace_ir += "%%%%%%%%%% STRUCTURAL ELEMENTS %%%%%%%%%%\n"
		bad_ace_ir += "%%%%%%%%%% STRUCTURAL ELEMENTS %%%%%%%%%%\n"
		for method in method_dict:
			good_ace_ir += "method(" + method + ").\n"
			bad_ace_ir += "method(" + method + ").\n"

		for method in method_dict:
			control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
			for bblock in control_flow_dict:
				good_ace_ir += "basic_block(" + method + "_" + bblock + ").\n"
				bad_ace_ir += "basic_block(" + method + "_" + bblock + ").\n"

		good_ace_ir += "\n\n\n\n\n\n\n\n\n"
		good_ace_ir += "%%%%%%%%%% STRUCTURE %%%%%%%%%%\n"
		bad_ace_ir += "\n\n\n\n\n\n\n\n\n"
		bad_ace_ir += "%%%%%%%%%% STRUCTURE %%%%%%%%%%\n"

		for method in method_dict:
			control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
			for bblock in control_flow_dict:
				good_ace_ir += "in(" + method + "_" + bblock + ", " + method + ").\n"
				bad_ace_ir += "in(" + method + "_" + bblock + ", " + method + ").\n"


		good_ace_ir += "\n\n\n\n"
		bad_ace_ir += "\n\n\n\n"


###############################################################################################


		for method in method_dict:
			## Then analyse the structure of each method.
			control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
			for bblock in control_flow_dict:
				for source in control_flow_dict[bblock]:
					good_ace_ir += "directed_edge(" + method + "_" + str(source) + ", " + method + "_" + bblock + ").\n"
					bad_ace_ir += "directed_edge(" + method + "_" + str(source) + ", " + method + "_" + bblock + ").\n"


		good_ace_ir += "\n\n\n\n"
		bad_ace_ir += "\n\n\n\n"


###############################################################################################
	
		for method in method_dict:
			for line in method_dict[method].splitlines():
				if re.match("(.*)\);", line):
					method_name = line.split()
					if(len(method_name) == 2):
						good_ace_ir += "method_call(" + method + ", " + method_name[0] + ").\n"
						bad_ace_ir += "method_call(" + method + ", " + method_name[0] + ").\n"

		good_ace_ir += "\n\n\n\n"
		bad_ace_ir += "\n\n\n\n"



		good_ace_ir += "end(model(" + subdirectory + "eff" + ")).\n\n\n\n\n\n"
		bad_ace_ir += "end(model(" + subdirectory + "ineff" + ")).\n"
		write_to_file(source_directory, subdirectory, good_ace_ir, bad_ace_ir)

		return good_ace_ir, bad_ace_ir



def write_to_file(source_directory, subdirectory, good_ace_ir, bad_ace_ir):
	# Print the output
	outfile_good = os.path.join(source_directory, subdirectory, "good_processed_ir.P")
	outfile_bad = os.path.join(source_directory, subdirectory, "bad_processed_ir.P")
	file_good = open(outfile_good, 'w')
	file_bad = open(outfile_bad, 'w')
	file_good.write(good_ace_ir)
	file_bad.write(bad_ace_ir)

def parse_function_name(line):
	processed_string = line.split()
	return processed_string[0]


