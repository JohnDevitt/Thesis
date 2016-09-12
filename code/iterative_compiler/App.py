
#!/usr/bin/env python

import os
import iterative_compiler
import report_generator.app as rg
import config


def main():

	subdirectories = [ name for name in os.listdir(config.beebs_directory) if os.path.isdir(os.path.join(config.beebs_directory, name)) ]


	for directory in subdirectories:
		print "Computing for directory: " + directory
		iterative_compiler.main(os.path.join(config.beebs_directory, directory), directory, config.output_directory)
		#rg.main(output_directory)


if __name__ == '__main__':
	main()