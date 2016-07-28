
#!/usr/bin/env python

import compilation_report_reader
import chart_generator
import pandas as pd 
import dropbox
import os

def main():

	source_directory = '/home/john/Thesis/beebs/src'

	dataframe = compilation_report_reader.parse_best_runtimes(source_directory)
	chart_generator.plot_categorical_data(dataframe, "best_runtimes")

	dataframe = compilation_report_reader.parse_all_runtimes(source_directory)
	chart_generator.plot_categorical_data(dataframe, "all_runtimes")

	app_key = 'bp74ulr6bwxi0te'
	app_secret = 'iu81rfk9bjwj7uq'

	client = dropbox.client.DropboxClient('4uCe0BWvzBUAAAAAAAAI08NiycJ_EzMvE8JikvJNIIqjbkR7tukU1Z_bjlAIAttF')

	f = open(os.path.join('/home/john/Thesis/ir/plots', 'best_runtimes.png'), 'rb')
	response = client.put_file('/best_runtimes.png', f, overwrite=True)
	f.close()

	f = open(os.path.join('/home/john/Thesis/ir/plots', 'all_runtimes.png'), 'rb')
	response = client.put_file('/all_runtimes.png', f, overwrite=True)
	f.close()

if __name__ == '__main__':
	main()