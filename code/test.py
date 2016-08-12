
import re


filename = "/home/john/Thesis/output/training-data/optimisation-level/train.kb"
raw_ir = open(filename, "r").readlines()

for line in raw_ir:
	p = re.compile("(.+)\((.+),(.+)\).$")
	res = p.findall(line)
	if(len(res) > 0):
		if(res[0][1] is res[0][2]):
			print res