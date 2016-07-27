
#!/usr/bin/env python

import random
import copy
from Configuration import Configuration

class ConfigurationList:
	def __init__(self):
		self.configurations = []

	def build_configurations(self, iterative_compilation_depth, flags):
		for index in range(0, iterative_compilation_depth):
			optimisation = random.choice(flags.optimisations)
			optimisation_flags = flags.flag_map[optimisation]
			configuration = Configuration()
			configuration.add_flag("-" + optimisation)

			for flag in optimisation_flags:
				if(random.uniform(0, 1) > 0.5):
					configuration.add_flag(flag)
					
			self.configurations.append(configuration)