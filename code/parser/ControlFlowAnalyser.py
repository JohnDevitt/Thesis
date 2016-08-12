#!/usr/bin/env python

import re

def main(raw_ir):

	return initial_bb_processing(raw_ir)


def initial_bb_processing(raw_ir):

	relational_dictionary = find_relations(raw_ir)
	control_flow_dictionary = build_initial_dictionary(raw_ir)

	for key in relational_dictionary.keys():
		control_flow_dictionary[int(relational_dictionary[key])] = []

	control_flow_dictionary = populate_with_goto_calls(raw_ir, control_flow_dictionary, relational_dictionary)
	control_flow_dictionary = populate_with_fallthrus(raw_ir, control_flow_dictionary, relational_dictionary)

	return control_flow_dictionary

def build_initial_dictionary(raw_ir):

	control_flow_dictionary = {}

	control_flow_dictionary[0] = []

	for line in raw_ir.splitlines():
		if re.match("(.*)<bb \D*(.*)>:", line):
			basic_block_number = int(re.findall(r'\d+', line)[0])
			control_flow_dictionary[basic_block_number] = []

	return control_flow_dictionary

def populate_with_goto_calls(raw_ir, control_flow_dictionary, relational_dictionary):

	current_basic_block_number = 0

	for line in raw_ir.splitlines():
		#if re.match ("(.*):", line) and not re.match("(.*)<bb \D*(.*)>:", line):
		#	if line[:-1] in relational_dictionary.keys():
		#		current_basic_block_number = int(relational_dictionary[line[:-1]])
		if re.match("(.*)<bb \D*(.*)>:", line):
			current_basic_block_number = int(re.findall(r'\d+', line)[0])
		elif re.match("(.*)goto <bb \D*(.*)>;", line):
			target_basic_block_number = int(re.findall(r'\d+', line)[0])
			control_flow_dictionary[current_basic_block_number].append(target_basic_block_number)

	return control_flow_dictionary

def populate_with_fallthrus(raw_ir, control_flow_dictionary, relational_dictionary):

	current_basic_block_number = 0
	trailing_goto_statement = False;

	for line in raw_ir.splitlines():
		if re.match ("(.*):", line) and not re.match("(.*)<bb \D*(.*)>:", line):
			if line[:-1] in relational_dictionary.keys():
				print "BOOOOOOO"
				current_basic_block_number = int(relational_dictionary[line[:-1]])
		elif ( re.match("(.*)<bb \D*(.*)>:", line) and trailing_goto_statement == False ):
			next_basic_block_number = int(re.findall(r'\d+', line)[0])
			control_flow_dictionary[current_basic_block_number].append(next_basic_block_number)
			current_basic_block_number = next_basic_block_number
			trailing_goto_statement = False
		elif re.match("(.*)goto <bb \D*(.*)>;", line):
			trailing_goto_statement = True
		elif re.match("(.+)", line):
			trailing_goto_statement = False

		if ( re.match("(.*)<bb \D*(.*)>:", line) ):
			current_basic_block_number = int(re.findall(r'\d+', line)[0])		


	return control_flow_dictionary

def find_relations(raw_ir):

	relation_dict = {}

	for line in raw_ir.splitlines():
		if re.match("(.*)<bb \D*(.*)> ((.+));", line):
			#print line
			p = re.compile("(.*)<bb \D*(.*)> \((.+)\);")
			res = p.findall(line)
			relation_dict[res[0][2]] = res[0][1]

	return relation_dict


if __name__ == '__main__':
	main()