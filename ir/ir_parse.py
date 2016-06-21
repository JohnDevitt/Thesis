#!/usr/bin/env python

import re
import ControlFlowAnalyser


def main():
	current_bb = 0;
	number_of_bb = 0;
	edge_count = 0;
	stmt_count = 0;
	expression_count = 0;	

	bb_dict = {}

	filename = "test.c.018t.ssa"

	raw_ir = open(filename, "r")
	# The processed internal program representation will begin with the function name, the number of basic blocks,
	# and the number of edges, as these are counted throughout the function, we'll append these at the end.
	processed_ir = "";

	control_flow_dict = ControlFlowAnalyser.main(filename)
	print control_flow_dict

	# We'll now loop through the file and parse it, line by line.
	for line in raw_ir:
		current_processed_line = ""
		if basic_block(line):
			current_bb, current_processed_line = parse_basic_block(line)
			number_of_bb = number_of_bb + 1

		processed_ir += current_processed_line

	print processed_ir



def basic_block(line):

	processed_line = ""

	if re.match("(.*)<bb \D*(.*)>:", line):
		bb_num = re.findall(r'\d+', line)
		processed_line = "bb_p(bb" + bb_num[0] + ").\n"
		processed_line += "% bb start %\n"
		processed_line += "% bb end %\n"

	return 0, processed_line


def if_stmt(line):
	if re.match("(.*)if(.*)", line):
		return True
	else:
		return False

def parse_if(line, stmt_count, expression_count):
	processed_line = "% start if stmt %\n"
	processed_line += "stmt_code(st"  +str(stmt_count) + ", gimple_cond).\n"
	processed_line += "cond_op(st" + str(stmt_count) + ", e" +  str(expression_count) + "). % Left operand % \n"
	expression_count = expression_count + 1
	processed_line += "cond_op(st" + str(stmt_count) + ", e" +  str(expression_count) + "). % Right operand % \n"
	expression_count = expression_count + 1
	processed_line += "cond_true(st"+ str(stmt_count) + ", e" +  str(expression_count) + ").\n"
	expression_count = expression_count + 1
	processed_line += "cond_false(st"+ str(stmt_count) + ", e" +  str(expression_count) + ").\n"
	expression_count = expression_count + 1
	processed_line += "% end if stmt %\n"
	return expression_count, processed_line



if __name__ == '__main__':
    main()