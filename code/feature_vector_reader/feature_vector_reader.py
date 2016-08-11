
import os
import numpy
import pickle
from sklearn.neighbors import KNeighborsClassifier

total = 0
count = 0


folders = [ name for name in os.listdir('/home/john/Desktop/cbeebs-ft-extract/src')
		if os.path.isdir(os.path.join('/home/john/Desktop/cbeebs-ft-extract/src', name)) ]


for target in folders:


	filepath = os.path.join('/home/john/Desktop/cbeebs-ft-extract/src', target, 'ici_features_function.ft')
	features_file = open(filepath, 'rb')
	features = features_file.read()
	features_file.close()

	if len(features) > 1:

		total = total + 1

		## This is a list of all the programs in beebs
		subdirectories = [ name for name in os.listdir('/home/john/Desktop/cbeebs-ft-extract/src')
				if os.path.isdir(os.path.join('/home/john/Desktop/cbeebs-ft-extract/src', name)) ]

		subdirectories.remove(target)

		features_list = []
		flags = []

		for directory in subdirectories:

			filepath = os.path.join('/home/john/Desktop/cbeebs-ft-extract/src', directory, 'ici_features_function.ft')
			features_file = open(filepath, 'rb')
			features = features_file.read()
			features_file.close()

			if len(features) > 1:

				if os.path.isfile(os.path.join('/home/john/Thesis/output/reports', directory, "compilation_report.pickle")):
					with open(os.path.join('/home/john/Thesis/output/reports', directory, "compilation_report.pickle"), 'rb') as input_file:
						compilation_report = pickle.load(input_file)

					flag = compilation_report['Configuration'][0]

					if(flag == "-O2"):
						flags.append("two")
					else:
						flags.append("three")

					filepath = os.path.join('/home/john/Desktop/cbeebs-ft-extract/src', directory, 'ici_features_function.ft')
					features_file = open(filepath, 'rb')
					features = features_file.read().split()
					features = [feature[2:].split('=') for feature in features]
					features = sorted([[ float(feature[0]), float(feature[1].replace(",", "")) ] for feature in features])
					features = [ feature[1] for feature in features ]
					features_list.append(features)
					features_file.close()


		X = numpy.array(features_list)
		y = numpy.array(flags)

		clf = KNeighborsClassifier(n_neighbors=1)
		clf.fit(X, y)

		example = open('/home/john/Desktop/cbeebs-ft-extract/src/' + target + '/ici_features_function.ft', 'rb')
		features = example.read().split()
		features = [feature[2:].split('=') for feature in features]
		features = sorted([[ float(feature[0]), float(feature[1].replace(",", "")) ] for feature in features])
		features = [ feature[1] for feature in features ]


		if os.path.isfile(os.path.join('/home/john/Thesis/output/reports', target, "compilation_report.pickle")):
			with open(os.path.join('/home/john/Thesis/output/reports', target, "compilation_report.pickle"), 'rb') as input_file:
				compilation_report = pickle.load(input_file)

			flag = compilation_report['Configuration'][0]
			#print flag, ", ", clf.predict(features)

			if(flag == "-O2"):
				flg = "two"
			else:
				flg = "three"


		if flg == clf.predict(features):
			count = count + 1

print count
print total

print float((float(count)/float(total))) * 100

