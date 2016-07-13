
import re


def build_method_dict(processed_ir):

	method_name = ""
	method_directory = {}
	line_count = 0

	previous_line = ""
	bracket_count = 0;
	balanced_brackets = True

	for line in processed_ir:
		line_count = line_count + 1
		bracket_count = bracket_count + count_brackets(line)

		# Implies we're starting a new method
		if(balanced_brackets == True and bracket_count == 1):
			balanced_brackets = False
			method_name = extract_method_name(previous_line)
			method_directory[method_name] = previous_line

		elif(balanced_brackets == False and bracket_count == 0):
			balanced_brackets = True
			method_directory[method_name] += line

		elif(balanced_brackets == False):
			method_directory[method_name] += line


		previous_line = line
		
	return method_directory



def count_brackets(line):

	count = 0;

	for character in line:
		if character is '{':
			count = count + 1
		if character is '}':
			count = count - 1

	return count

def extract_method_name(line):
	processed_string = line.split()
	return processed_string[0]



'''

	text = method_directory['BubbleSort']
	for line in text.splitlines():
		print line




	'''