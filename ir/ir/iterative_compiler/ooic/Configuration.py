
#!/usr/bin/env python

import numpy

class Configuration:
	def __init__(self):
		self.configuration = []

	def compile(self, directory, filename):
		compile_string = os.path.join(directory, filename)

	def run(self, directory, filename):
		run_string = os.path.join(directory, filename)

	def generate_median_runtime(self, directory, filename):

		runtimes = []

		for i in range(0, 10):
			self.compile()
			runtime.append(self.run())

		return numpy.median(runtimes)

	def add_flag(self, flag):
		self.configuration.append(flag)

