
#!/usr/bin/env python

import os
import iterative_compiler
import report_generator.app as rg


def main():
#def main():

	subdirectories = [ name for name in os.listdir("/home/john/Thesis/beebs/src/") if os.path.isdir(os.path.join("/home/john/Thesis/beebs/src/", name)) ]

	count = 0

	for directory in subdirectories:
		count = count + 1
		print directory
		print count


if __name__ == '__main__':
	main()