#!/usr/bin/env python

import re


def main():
	current_bb = 0;
	number_of_bb = 0;
	edge_count = 0;
	stmt_count = 0;

	raw_ir = open("test.c.018t.ssa", "r")
	# The processed internal program representation will begin with the function name, the number of basic blocks,
	# and the number of edges, as these are counted throughout the function, we'll append these at the end.
	processed_ir = "";

	# We'll now loop through the file and parse it, line by line.
	for line in raw_ir:
		current_processed_line = ""
		if basic_block(line):
			current_bb, current_processed_line = parse_basic_block(line)
			number_of_bb = number_of_bb + 1
		elif goto(line):
			current_processed_line = parse_goto(line, current_bb, edge_count)
			edge_count = edge_count + 1
		elif if_stmt(line):
			current_processed_line = parse_if(line, stmt_count)
			stmt_count = stmt_count + 1

		processed_ir += current_processed_line

	print processed_ir

def basic_block(line):
	if re.match("(.*)<bb \D*(.*)>:", line):
		return True
	else:
		return False

def parse_basic_block(line):
	bb_num = re.findall(r'\d+', line)
	processed_line = "bb(" + bb_num[0] + ").\n"
	return bb_num, processed_line

def goto(line):
	if re.match("(.*)goto <bb \D*(.*)>;", line):
		return True
	else:
		return False

def parse_goto(line, current_bb, edge_count):
	bb_num = re.findall(r'\d+', line)
	processed_line = "edge_src(ed" + str(edge_count) + ", bb" + str(current_bb[0]) + ").\n"
	processed_line += "edge_dest(ed" + str(edge_count) + ", bb" + str(bb_num[0]) + ").\n"
	return processed_line

def if_stmt(line):
	if re.match("(.*)if(.*)", line):
		return True
	else:
		return False

def parse_if(line, stmt_count):
	processed_line = "% start if stmt %\n"
	processed_line += "stmt_code("  +str(stmt_count) + ", gimple_cond).\n"
	processed_line += "cond_true("+ str(stmt_count) + ", number)."
	processed_line += "cond_false("+ str(stmt_count) + ", number)."
	processed_line += "% end if stmt %\n"
	return processed_line



if __name__ == '__main__':
    main()