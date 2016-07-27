
#!/usr/bin/env python

import subprocess
import timeit
import numpy

def compile_program(command):

	p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)
	p.communicate()[0]

	if (p.returncode > 1):
		return False
	return True

def run_program(path):


	command = 'p = subprocess.Popen(\'time ' + path + '\', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE); out, err = p.communicate()'

	result = timeit.Timer(command, setup='import subprocess').repeat(1, 10)
	return numpy.median(result)




