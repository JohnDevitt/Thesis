
import os
import string

def extractModel(output_directory, directory_name):

	model = ""

	filepath = os.path.join(output_directory, "training-data", directory_name, "tilde", "train.out")
	with open(filepath, 'r') as f:
		for line in f:
			if "Equivalent prolog program:" in line:                
				for line in f:
					model += line

	model = model.replace("class(", "\nclass(")
	model = model.replace(" :- ", " :-\ncurrent_predicate(conditional_edge, _),current_predicate(method_call, _),")
	model = model.replace("),", "),\n")
	model = model.replace(" !.", "!.")


	#model = model.replace("current_predicate", "\tcurrent_predicate")
	#model = model.replace("conditional_edge", "\tconditional_edge")
	#model = model.replace("directed_edge", "\tdirected_edge")
	#model = model.replace("method_call", "\tmethod_call")
	#model = model.replace("method", "\tmethod")

	return model