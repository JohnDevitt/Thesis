
#!/usr/bin/env python

import compilation_report_reader
import chart_generator
import pandas as pd 

def main():

	source_directory = '/home/john/Thesis/beebs/src'

	dataframe = compilation_report_reader.parse_best_runtimes(source_directory)
	chart_generator.plot_categorical_data(dataframe, "best_runtimes")

	dataframe = compilation_report_reader.parse_all_runtimes(source_directory)
	chart_generator.plot_categorical_data(dataframe, "all_runtimes")

if __name__ == '__main__':
	main()