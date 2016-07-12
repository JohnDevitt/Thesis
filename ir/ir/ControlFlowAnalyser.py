#!/usr/bin/env python

import re

def main(filename):

	raw_ir = open(filename, "r")
	return initial_bb_processing(raw_ir)


def initial_bb_processing(raw_ir):
	
	control_flow_dictionary = build_initial_dictionary(raw_ir)
	control_flow_dictionary = populate_with_goto_calls(raw_ir, control_flow_dictionary)
	control_flow_dictionary = populate_with_fallthrus(raw_ir, control_flow_dictionary)

	return control_flow_dictionary

def build_initial_dictionary(raw_ir):

	control_flow_dictionary = {}

	for line in raw_ir:
		if re.match("(.*)<bb \D*(.*)>:", line):
			basic_block_number = re.findall(r'\d+', line)[0]
			control_flow_dictionary[basic_block_number] = []

	raw_ir.seek(4)
	return control_flow_dictionary

def populate_with_goto_calls(raw_ir, control_flow_dictionary):

	current_basic_block_number = 0

	for line in raw_ir:
		if re.match("(.*)<bb \D*(.*)>:", line):
			current_basic_block_number = re.findall(r'\d+', line)[0]
		elif re.match("(.*)goto <bb \D*(.*)>;", line):
			target_basic_block_number = re.findall(r'\d+', line)[0]
			control_flow_dictionary[target_basic_block_number].append(current_basic_block_number)

	raw_ir.seek(4)
	return control_flow_dictionary

def populate_with_fallthrus(raw_ir, control_flow_dictionary):

	current_basic_block_number = 0
	trailing_goto_statement = False;

	for line in raw_ir:
		if ( re.match("(.*)<bb \D*(.*)>:", line) and trailing_goto_statement == False ):
			next_basic_block_number = re.findall(r'\d+', line)[0]
			control_flow_dictionary[next_basic_block_number].append(current_basic_block_number)
			current_basic_block_number = next_basic_block_number
			trailing_goto_statement = False
		elif re.match("(.*)goto <bb \D*(.*)>;", line):
			trailing_goto_statement = True
		elif re.match("(.+)", line):
			trailing_goto_statement = False

	raw_ir.seek(4)
	return control_flow_dictionary


if __name__ == '__main__':
	main()