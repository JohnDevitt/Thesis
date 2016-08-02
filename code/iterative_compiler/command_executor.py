
#!/usr/bin/env python

import subprocess
import timeit
import numpy
import runner
import os
import analysis_report_reader

def compile_program(command):

	print "============================================================="
	print command

	p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)
	p.communicate()[0]

	if (p.returncode > 1):
		return False
	return True

def run_program(path, output_directory, filename):

	#subprocess.Popen("time " + path, shell=True, stdout=subprocess.PIPE)
	directory_path = os.path.join(output_directory, "reports", filename)
	if not os.path.exists(directory_path):
		os.makedirs(directory_path)

	outfile = os.path.join(output_directory, "reports", filename, "analysis.txt")

	times = []

	for i in range (0, 10):
		subprocess.Popen(path, shell=True, stdout=subprocess.PIPE).wait()
		subprocess.Popen("gprof " + path + " /home/john/Thesis/code/gmon.out > " + outfile, shell=True, stdout=subprocess.PIPE).wait()
		times.append(analysis_report_reader.read_run_time('/home/john/Thesis/output/reports/', filename))

	print times
	print numpy.median(times)
	print "-------------------------------------------------------------"

	return numpy.median(times)