
#!/usr/bin/env python

import subprocess
import timeit
import numpy
import runner

def compile_program(command):

	p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)
	p.communicate()[0]

	if (p.returncode > 1):
		return False
	return True

def run_program(path):

	#subprocess.Popen("time " + path, shell=True, stdout=subprocess.PIPE)

	times = [runner.timer(path) for num in range(0, 10)]
	return numpy.median(times)




