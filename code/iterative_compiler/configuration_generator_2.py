
'''
This file generates a list of compiler confogurations that can be applied to each program in
a benchmark dataset. The size of the list that it generates is user defined. This list can then
be used in conjunction with an iterative compilation framework
'''


#!/usr/bin/env python

import pandas as pd
import random as rand

def generate_flags():

	optimisations = ["-O2", "-O3"]
	flags = ["-fipa-pta", "-ftree-loop-if-convert-stores", "-ftree-loop-im", "-ftree-loop-ivcanon", "-ftree-vectorize", "-fstrict-aliasing", "-fbtr-bb-exclusive", "-ffast-math", "-funroll-loops"]

	configurations = []

	for i in range(0, 100):
		configuration = [rand.choice(optimisations)]
		for flag in flags:
			if rand.uniform(0, 1) > 0.5:
				configuration.append(flag)

		configurations.append(configuration)

	return configurations

if __name__ == "__main__":

	print generate_flags()