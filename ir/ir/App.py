#!/usr/bin/env python

import os
import IterativeCompiler
import IntermediateRepresentationGenerator
import IntermediateRepresentationParser

source_directory = "/home/john/Thesis/beebs/src"
flag_database = "/home/john/Thesis/ir/ir/arm-none-eabi-gcc-4.8.2-flags.csv"
subdirectories = [ name for name in os.listdir(source_directory) if os.path.isdir(os.path.join(source_directory, name)) ]


def main():

	iterative_compilation_depth = 1000

	for subdirectory in subdirectories:
		IterativeCompiler.main(source_directory, subdirectory, flag_database, iterative_compilation_depth)
		IntermediateRepresentationGenerator.main(source_directory, subdirectory)
		IntermediateRepresentationParser.main(source_directory, subdirectory)

if __name__ == '__main__':
	main()