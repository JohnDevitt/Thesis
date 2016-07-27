
#!/usr/bin/env python

import subprocess
import time

def compile_program(compile_command):

	successful = make_system_call(compile_command)	

def run_program(run_command):
	
	end = 0
	start = 0

	start = time.time()
	successful = make_system_call(run_command)
	end = time.time()

	runtime = end - start

	if successful:
		return runtime
	else:
		return float('inf')


def make_system_call(command):
	p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)
	p.communicate()[0]

	if (p.returncode > 1):
		return False
	return True


