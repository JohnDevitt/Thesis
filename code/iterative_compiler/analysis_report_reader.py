
import os
import re


def read_run_time(output_directory, filename):

	final_line = ""

	with open(os.path.join(output_directory,  filename, "analysis.txt"), 'rb') as input_file:

		for line in input_file:
			if(re.match(("\s*\d+\.\d+\s+\d+\.\d+\s+\d+\.\d+(.*)$"), line)):
				final_line = line

	if final_line is "":
		return 0

	return float( final_line.split()[1] )