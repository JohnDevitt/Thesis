
#!/usr/bin/env python

import os
import pickle

def generate_report(baseline_runtime, best_configuration, best_runtime, directory, output_directory):

	directory_path = os.path.join(output_directory, "reports", directory)
	if not os.path.exists(directory_path):
		os.makedirs(directory_path)

	increase = best_runtime < baseline_runtime
	difference = abs(best_runtime - baseline_runtime)

	if (baseline_runtime == 0) :
		proportion = 0
	else:
		proportion = (difference / baseline_runtime) * 100

	## Build the knowledge base
	filepath = os.path.join(directory_path, "compilation_report.txt")
	file = open(filepath, 'w')

	file.write("\n\n")
	file.write("Baseline Configuration Speed(-03 only): ")
	file.write(str(baseline_runtime))
	file.write("\nBest Configuration Speed: ")
	file.write(str(best_runtime))

	if(increase):
		file.write("\nThat's an increase of: ")
		file.write(str(difference))
		file.write("\n")
		file.write("That's ")
		file.write(str(proportion))
		file.write("% better than the basline")
	else:
		file.write("\nThat's a decrease of: ")
		file.write(str(difference))
		file.write("\n")
		file.write("That's ")
		file.write(str(proportion))
		file.write("% worse than the basline")

	file.write("\n\n")
	file.write("Best Configuration: ")
	file.write(str(best_configuration))
	file.write("\n\n")

	file.write("\n\n")
	file.close()

	report = {'Best': best_runtime, 'Baseline': baseline_runtime, 'Difference': difference,
			'Proportion': proportion, 'Configuration': best_configuration}

	with open(os.path.join(directory_path, "compilation_report.pickle"), 'w') as f:
		pickle.dump(report, f)

	return report