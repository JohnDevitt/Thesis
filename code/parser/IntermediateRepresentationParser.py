#!/usr/bin/env python

import re
import ControlFlowAnalyser
import MethodSeperator
import FlagParser
import os
import inflect
import glob




def main(source_directory, subdirectory, classification):

	prolog_ir = build_ir_header(source_directory, subdirectory, classification)

	for file in glob.glob(source_directory + "/" + subdirectory + "/" + "*.cfg"):
		filename = extract_filename(subdirectory, file)
		if not filename == "main" and not filename == "boardsupport":
			
			raw_ir = open(file, "r")
			processed_ir = []
			processed_ir = raw_ir.readlines()
			raw_ir.close

			## First, break the code down into it's seperate methods.
			method_dict = MethodSeperator.build_method_dict(processed_ir)

			prolog_ir += parse_structural_elements(method_dict, filename)
			prolog_ir += parse_structural_connections(method_dict, filename)

	prolog_ir += build_ir_footer(subdirectory)

	return prolog_ir

###############################################################################################

	#for method in method_dict:
		#for line in method_dict[method].splitlines():


def parse_function_name(line):
	processed_string = line.split()
	return processed_string[0]

def generate_basic_block_variable_name(basic_block, method, file):
	p = inflect.engine()
	return p.number_to_words(str(basic_block)).replace(" ", "") + str(method) + str(file)

def generate_method_variable_name(method, file):
	return method + file

def build_ir_header(source_directory, subdirectory, classification):
	header = "\nbegin(model(" + subdirectory + ")).\n"

	## Special cases
	if(subdirectory == "2dfir"):
		header = "\nbegin(model(twodfir)).\n"		
	if(subdirectory == "newlib-mod"):
		header = "\nbegin(model(newlibmod)).\n"

	header += str(classification).lower() + ".\n"

	header += "%%%%%%%%%% STRUCTURAL ELEMENTS %%%%%%%%%%\n"

	for file in glob.glob(source_directory + "/" + subdirectory + "/" + "*.cfg"):
		filename = extract_filename(subdirectory, file)
		if not filename == "main" and not filename == "boardsupport":
			header += "file(" + filename + "file).\n"

	return header

def build_ir_footer(subdirectory):

	footer = "end(model(" + subdirectory + ")).\n\n"

	if(subdirectory == "2dfir"):
		footer = "end(model(twodfir)).\n\n"
	elif(subdirectory == "newlib-mod"):
		footer = "end(model(newlibmod)).\n\n"

	return footer

def extract_filename(subdirectory, file):
	p = re.compile("/home/john/Thesis/beebs/src/" + subdirectory + "/(.*).c.011t.cfg")
	res = p.findall(file)
	return res[0].replace(".", "")

def parse_structural_elements(method_dict, file):

	elements = ""

	## List the Methods.
	for method in method_dict:
		elements += "method(" + method + ").\n"

	## List the basic blocks.
	for method in method_dict:
		control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
		for basic_block in control_flow_dict:
			basic_block_varible_name = generate_basic_block_variable_name(basic_block ,method, file)
			elements += "basic_block(" + basic_block_varible_name + ").\n"

	return elements

def parse_structural_connections(method_dict, file):

	structure = "%%%%%%%%%% STRUCTURE %%%%%%%%%%\n"
	structure += link_basic_blocks_to_methods(method_dict, file)
	structure += link_basic_blocks_to_basic_blocks(method_dict, file)
	structure += link_method_calls_and_return_statements(method_dict, file)

	return structure

def link_basic_blocks_to_methods(method_dict, file):

	link = ""

	## Link basic blocks to methods.
	for method in method_dict:
		control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
		for basic_block in control_flow_dict:
			basic_block_variable_name = generate_basic_block_variable_name(basic_block ,method, file)
			method_variable_name = generate_method_variable_name(method, file)
			link += "in(" + basic_block_variable_name + ", " + method_variable_name + ").\n"

	return link

def link_basic_blocks_to_basic_blocks(method_dict, file):

	link = ""

	## Link the basic blocks to each other
	for method in method_dict:
		control_flow_dict = ControlFlowAnalyser.main(method_dict[method])
		for basic_block in control_flow_dict:
			source_basic_block = generate_basic_block_variable_name(basic_block, method, file)
			
			if(len(control_flow_dict[basic_block]) > 1):
				for dest in control_flow_dict[basic_block]:
					destination_basic_block = generate_basic_block_variable_name(dest, method, file)
					link += "conditional_edge(" + source_basic_block + ", " + destination_basic_block + ").\n"
			
			elif(len(control_flow_dict[basic_block]) > 0):
				dest = control_flow_dict[basic_block][0]
				destination_basic_block = generate_basic_block_variable_name(dest, method, file)
				link += "directed_edge(" + source_basic_block + ", " + destination_basic_block + ").\n"

	return link

def link_method_calls_and_return_statements(method_dict, file):

	link = ""

	current_basic_block_number = 0
	for method in method_dict:
		for line in method_dict[method].splitlines():
			## Keep bb updated
			if re.match("(.*)<bb \D*(.*)>:", line):
				current_basic_block_number = re.findall(r'\d+', line)[0]
			## Search for method calls
			pattern = r'(\w*) \((.*)\);$'
			match = re.search(pattern, line)
			if match:
				method_name = list(match.groups())[0]
				if(method_name in method_dict.keys()):
					source_basic_block = generate_basic_block_variable_name(current_basic_block_number, method, file)
					destination_basic_block = generate_basic_block_variable_name(2, method_name, file)
					link += "method_call(" + source_basic_block + ", " + destination_basic_block + ").\n"
					control_flow_dict = ControlFlowAnalyser.main(method_dict[method_name])
					returning_basic_block = generate_basic_block_variable_name(max(sorted(control_flow_dict.keys())), method_name, file)
					link += "return_statement(" + returning_basic_block + ", " + source_basic_block + ").\n"

	return link