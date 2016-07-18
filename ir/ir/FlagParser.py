
import os
import csv

def main(directory, subdirectory):


	f = open('/home/john/Thesis/ir/ir/arm-none-eabi-gcc-4.8.2-flags.csv', 'rb') # opens the csv file
	try:
		reader = csv.reader(f)  # creates the reader object
		flags = [row[0].replace(" ", "") for row in reader if row[0] and row[0] != "Flag"]
	finally:
		f.close()      # closing

	print flags


	filepath = os.path.join(directory, subdirectory, "compilation_report.txt")
	if os.path.exists(filepath):
		raw_ir = open(filepath, "r")
		processed_ir = raw_ir.readlines()

		good_config = ""
		bad_config = ""

		for item in flags:
			if item in processed_ir[7].split()[9: len(processed_ir[7].split()) - 2]:
				good_config += item + "(on).\n"
			else:
				good_config += item + "(off).\n"

		for item in flags:
			if item in processed_ir[7].split()[9: len(processed_ir[7].split()) - 2]:
				bad_config += item + "(on).\n"
			else:
				bad_config += item + "(off).\n"

		return good_config, bad_config