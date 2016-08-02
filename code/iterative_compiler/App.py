
#!/usr/bin/env python

import os
import iterative_compiler
import report_generator.app as rg


def main(source_directory, flag_database, iterative_compilation_depth, output_directory):
#def main():

	subdirectories = [ name for name in os.listdir(source_directory) if os.path.isdir(os.path.join(source_directory, name)) ]

	#iterative_compiler.main('/home/john/Thesis/beebs/src/nsichneu', 'nsichneu', '/home/john/Thesis/ir/ir/arm-none-eabi-gcc-4.8.2-flags.csv', 100)

	reports = {}

	for directory in subdirectories[3:]:
		print "Computing for directory: " + directory
		iterative_compiler.main(os.path.join(source_directory, directory), directory, flag_database, iterative_compilation_depth, output_directory)
		rg.main(output_directory)


if __name__ == '__main__':
	main()