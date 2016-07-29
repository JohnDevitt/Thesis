
#!/usr/bin/env python

import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import os
import pandas as pd 
import seaborn as sns
sns.set(style="darkgrid", color_codes=True)

def plot_categorical_data(dataframe, filename, output_directory):

	## Flush
	sns.plt.clf()
	sns.plt.cla()
	sns.plt.close()

	## Plot
	plt.figure(figsize=(18.2, 10))
	ax = sns.stripplot(y=dataframe["Decrease"], x=dataframe["Program"], size=8)
	
	## Plot style
	plt.yticks(fontsize=15)
	plt.xticks(rotation=45, fontsize=7.5)
	plt.gca().yaxis.set_major_formatter(mticker.FormatStrFormatter('%d %%'))
	plt.suptitle('Decrease in runtime over the baseline.', fontsize = 24)
	
	## axhlines
	cutoff_upper = plt.axhline(10, color='r', linestyle='--', label='Upper Suggested Cut Off Point.', linewidth=1)
	baseline = plt.axhline(0, color='b', linestyle='--', label='Median Baseline Runtime.', linewidth=1)
		
	## Legend
	plt.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0, handles=[cutoff_upper, baseline], fontsize = 17)
	
	if not os.path.exists(os.path.join(output_directory, "plots")):
		os.makedirs(os.path.join(output_directory, "plots"))
	
	## Save it
	sns.plt.savefig(os.path.join(output_directory, "plots", filename), bbox_inches='tight')

	## Flush
	sns.plt.clf()
	sns.plt.cla()
	sns.plt.close()

	#plt.scatter(indicies, sorted_runtimes)
	#plt.gca().yaxis.set_major_formatter(mticker.FormatStrFormatter('%d %%'))
	#plt.ylabel('Increse over the baseline', fontsize=11)

	#plt.axhline(0, color='b', linestyle='--', label='Baseline')
	#plt.show()
