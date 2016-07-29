
import os
import csv



def main(flag_database):
	f = open(flag_database, 'rb') # opens the csv file
	try:
		reader = csv.reader(f)  # creates the reader object
		flags = [row[0].replace(" ", "") for row in reader if row[0] and row[0] != "Flag"]
	finally:
		f.close()      # closing


	#flags.insert(0, "one")
	#flags.insert(0, "two")
	#flags.insert(0, "three")

	return flags

if __name__ == '__main__':
	main()
