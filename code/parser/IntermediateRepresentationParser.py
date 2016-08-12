#!/usr/bin/env python

import re
import ControlFlowAnalyser
import MethodSeperator
import FlagParser
import os
import inflect




def main(source_directory, subdirectory, flag):



	## FILE IO
	filename = subdirectory + ".c.011t.cfg"
	filepath = os.path.join(source_directory, subdirectory, filename) 
	raw_ir = open(filepath, "r")
	processed_ir = raw_ir.readlines()
	ace_ir = "\nbegin(model(" + subdirectory + ")).\n"
	ace_ir += str(flag).lower() + ".\n"

	p = inflect.engine()


########################################################################
#######################

	ace_ir += "%%%%%%%%%% STRUCTURAL ELEMENTS %%%%%%%%%%\n"
	
	## First, break the code down into it's seperate methods.
	method_dict = MethodSeperator.build_method_dict(processed_ir)

	#for method in method_dict:
		#ace_ir += "method(" + method + ").\n"

	#for method in method_dict:
		#print method
		#control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
		#for bblock in control_flow_dict:
			#ace_ir += "basic_block(" + p.number_to_words(str(bblock)).replace(" ", "") + method + ").\n"

	ace_ir += "%%%%%%%%%% STRUCTURE %%%%%%%%%%\n"

	#for method in method_dict:
		#control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
		#for bblock in control_flow_dict:
			#ace_ir += "in(" + p.number_to_words(str(bblock)).replace(" ", "") + method + ", " + method + ").\n"


###############################################################################################


	for method in method_dict:
		## Then analyse the structure of each method.
		control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
		for bblock in control_flow_dict:
			if(len(control_flow_dict[bblock]) > 1):

				if len(control_flow_dict[bblock]) > 2:
					print "=============================="
					print "++++++++++++++++++++++++++++++"
					print len(control_flow_dict[bblock])
					print method
					print bblock
					print "++++++++++++++++++++++++++++++"
					print "=============================="

				for dest in control_flow_dict[bblock]:
					ace_ir += "conditional_edge(" + p.number_to_words(bblock).replace(" ", "") + method + ", " + p.number_to_words(str(dest)).replace(" ", "") + method + ").\n"
			elif(len(control_flow_dict[bblock]) > 0):
				dest = control_flow_dict[bblock][0]
				ace_ir += "directed_edge(" + p.number_to_words(bblock).replace(" ", "") + method + ", " + p.number_to_words(str(dest)).replace(" ", "") + method + ").\n"


###############################################################################################

	#print method_dict

	current_basic_block_number = 0

	for method in method_dict:
		for line in method_dict[method].splitlines():
			if re.match("(.*)<bb \D*(.*)>:", line):
				current_basic_block_number = re.findall(r'\d+', line)[0]
			
			pattern = r'(\w*) \((.*)\);$'
			match = re.search(pattern, line)
			if match:
				method_name = list(match.groups())[0]
				if(method_name in method_dict.keys()):
					ace_ir += "method_call(" + p.number_to_words(current_basic_block_number).replace(" ", "") + method + ", two" + method_name + ").\n"
					control_flow_dict = ControlFlowAnalyser.main(method_dict[method_name])
					ace_ir += "return_statement(" + p.number_to_words(str(max(sorted(control_flow_dict.keys())))).replace(" ", "") + method_name + ", " + p.number_to_words(current_basic_block_number).replace(" ", "") + method + ").\n"

	ace_ir += "end(model(" + subdirectory + ")).\n\n"
	return ace_ir

###############################################################################################

	#for method in method_dict:
		#for line in method_dict[method].splitlines():


def parse_function_name(line):
	processed_string = line.split()
	return processed_string[0]


