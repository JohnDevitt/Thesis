#!/usr/bin/env python

import re
import ControlFlowAnalyser
import MethodSeperator
import FlagParser
import os


def main(source_directory, subdirectory, flag, enabled):



	## FILE IO
	filename = subdirectory + ".c.018t.ssa"
	filepath = os.path.join(source_directory, subdirectory, filename) 
	raw_ir = open(filepath, "r")
	processed_ir = raw_ir.readlines()
	ace_ir = "\nbegin(model(" + subdirectory + ")).\n"
	ace_ir += str(enabled).lower() + ".\n"



###############################################################################################

	ace_ir += "%%%%%%%%%% STRUCTURAL ELEMENTS %%%%%%%%%%\n"
	
	## First, break the code down into it's seperate methods.
	method_dict = MethodSeperator.build_method_dict(processed_ir)

	for method in method_dict:
		ace_ir += "method(" + method + ").\n"

	for method in method_dict:
		control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
		for bblock in control_flow_dict:
			ace_ir += "basic_block(" + method + "_" + bblock + ").\n"

	ace_ir += "%%%%%%%%%% STRUCTURE %%%%%%%%%%\n"

	for method in method_dict:
		control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
		for bblock in control_flow_dict:
			ace_ir += "in(" + method + "_" + bblock + ", " + method + ").\n"


###############################################################################################


	for method in method_dict:
		## Then analyse the structure of each method.
		control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
		for bblock in control_flow_dict:
			for source in control_flow_dict[bblock]:
				ace_ir += "directed_edge(" + method + "_" + str(source) + ", " + method + "_" + bblock + ").\n"


###############################################################################################

	for method in method_dict:
		for line in method_dict[method].splitlines():
			if re.match("(.*)\);", line):
				method_name = line.split()
				if(len(method_name) == 2):
					ace_ir += "method_call(" + method + ", " + method_name[0] + ").\n"



	ace_ir += "end(model(" + subdirectory + ")).\n\n"
	return ace_ir


def parse_function_name(line):
	processed_string = line.split()
	return processed_string[0]


