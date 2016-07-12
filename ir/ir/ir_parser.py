#!/usr/bin/env python

import re
import ControlFlowAnalyser
import os


def main():

	# IR output from gcc
	directory = "/home/john/Thesis/beebs/src"
	subdirectories = [ name for name in os.listdir(directory) if os.path.isdir(os.path.join(directory, name)) ]

	for subdirectory in subdirectories:
		# Counter varibles
		edge_count = 0;
		#expression_count = 0;
		#stmt_count = 0;

		# Intermediate Representation file
		filepath = os.path.join(directory, subdirectory) + "/" + subdirectory + ".c.018t.ssa"
		
		# Sometimes it isn't generated for some reason
		if os.path.exists(filepath):
			raw_ir = open(filepath, "r")

			# Tabular representation of the programs control flow
			control_flow_dict = ControlFlowAnalyser.main(filepath)

			# Varible to hold the parsed result
			processed_ir = ""

			# We'll now loop through the file and parse it, line by line.
			for line in raw_ir:
				current_processed_line = ""
				if re.match("(.*)<bb \D*(.*)>:", line):
					current_processed_line, edge_count = basic_block(line, control_flow_dict, edge_count)
				#elif re.match("(.*)if(.*)", line):	
					#current_processed_line, stmt_count, expression_count = parse_if(line, stmt_count, expression_count)
				#elif re.match("(.*) = (.*)", line):
					#current_processed_line, stmt_count = assignment(line, stmt_count)
				processed_ir += current_processed_line

			# Print the output
			outfile = os.path.join(directory, subdirectory, "processed_ir.P")
			file = open(outfile, 'w')
			file.write(processed_ir)



def basic_block(line, control_flow_dict, edge_count):

	bb_num = re.findall(r'\d+', line)[0]
	processed_line = "% New Basic Block %\n"
	processed_line += "bb_p(bb" + bb_num + ").\n"
	processed_line += "% Here is the control flow info from this basic block %\n"

	for src in control_flow_dict[bb_num]:
		processed_line += "edge_src(ed" + str(edge_count) + ", bb" + str(src) + ").\n"
		processed_line += "edge_dest(ed" + str(edge_count) + ", bb" + str(bb_num) + ").\n"
		edge_count = edge_count + 1

	processed_line += "% bb End control flow info %\n"

	return processed_line, edge_count



def parse_if(line, stmt_count, expression_count):


	processed_line = "% start if stmt %\n"
	processed_line += "stmt_code(st"  +str(stmt_count) + ", gimple_cond).\n"
	stmt_count = stmt_count + 1
	processed_line += "cond_op(st" + str(stmt_count) + ", e" +  str(expression_count) + "). % Left operand % \n"
	expression_count = expression_count + 1
	processed_line += "cond_op(st" + str(stmt_count) + ", e" +  str(expression_count) + "). % Right operand % \n"
	expression_count = expression_count + 1
	processed_line += "cond_true(st"+ str(stmt_count) + ", e" +  str(expression_count) + ").\n"
	expression_count = expression_count + 1
	processed_line += "cond_false(st"+ str(stmt_count) + ", e" +  str(expression_count) + ").\n"
	expression_count = expression_count + 1
	processed_line += "% end if stmt %\n"
	return processed_line, stmt_count, expression_count

def assignment(line, stmt_count):

	processed_line = "% start assign stmt %\n"
	processed_line += "stmt_code(st"  +str(stmt_count) + ", gimple_assign).\n"
	processed_line += "stmt_flag(st"  +str(stmt_count) + ", has_mem_ops).\n"
	stmt_count = stmt_count + 1
	processed_line += "% end assign stmt %\n"
	return processed_line, stmt_count


if __name__ == '__main__':
	main()