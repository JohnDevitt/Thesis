#!/usr/bin/env python


import IntermediateRepresentationParser
import os


source_directory = "/home/john/Thesis/beebs/src"

subdirectories = [directory for directory in os.listdir(source_directory) if os.path.isdir(os.path.join(source_directory, directory))]

count = 0


for subdirectory in subdirectories:
	ret = IntermediateRepresentationParser.main(source_directory, subdirectory)
	count = count + 1


with open(os.path.join("/home/john/Thesis", "app.kb"), 'w') as outfile:
	for subdirectory in subdirectories:
		if os.path.exists(os.path.join(source_directory, subdirectory, "processed_ir.P")):
			with open(os.path.join(source_directory, subdirectory, "processed_ir.P")) as infile:
				outfile.write(infile.read())