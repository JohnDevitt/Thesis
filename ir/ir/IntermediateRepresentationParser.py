#!/usr/bin/env python

import re
import ControlFlowAnalyser
import MethodSeperator
import os


def main(source_directory, subdirectory):



	## FILE IO
	filepath = os.path.join(source_directory, subdirectory) + "/" + subdirectory + ".c.018t.ssa"
	if os.path.exists(filepath):
		raw_ir = open(filepath, "r")
		processed_ir = raw_ir.readlines()
		ace_ir = "\nbegin(model(" + subdirectory + ")).\n"
		ace_ir += "classification.\n"



		## First, break the code down into it's seperate methods.
		method_dict = MethodSeperator.build_method_dict(processed_ir)



###############################################################################################

		ace_ir += "%%%%%%%%%% STRUCTURAL ELEMENTS %%%%%%%%%%\n"
		for method in method_dict:
			ace_ir += "method(" + method + ").\n"

		for method in method_dict:
			control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
			for bblock in control_flow_dict:
				ace_ir += "basic_block(" + method + "_" + bblock + ").\n"

		ace_ir += "\n\n\n\n\n\n\n\n\n"
		ace_ir += "%%%%%%%%%% STRUCTURE %%%%%%%%%%\n"

		for method in method_dict:
			control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
			for bblock in control_flow_dict:
				ace_ir += "in(" + method + "_" + bblock + ", " + method + ").\n"

		ace_ir += "\n\n\n\n"

###############################################################################################


		for method in method_dict:
			## Then analyse the structure of each method.
			control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
			for bblock in control_flow_dict:
				for source in control_flow_dict[bblock]:
					ace_ir += "directed_edge(" + method + "_" + str(source) + ", " + method + "_" + bblock + ").\n"


		ace_ir += "\n\n\n\n"

###############################################################################################
	
		for method in method_dict:
			for line in method_dict[method].splitlines():
				print line
				if re.match("(.*)\);", line):
					method_name = line.split()
					ace_ir += "method_call(" + method + ", " + method_name[0] + ")."

		ace_ir += "\n\n\n\n"



		ace_ir += "end(model(" + subdirectory + ")).\n"
		write_to_file(source_directory, subdirectory, ace_ir)



def write_to_file(source_directory, subdirectory, processed_ir):
	# Print the output
	outfile = os.path.join(source_directory, subdirectory, "processed_ir.P")
	file = open(outfile, 'w')
	file.write(processed_ir)

def parse_function_name(line):
	processed_string = line.split()
	return processed_string[0]




if __name__ == '__main__':
	main()