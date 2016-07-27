
#!/usr/bin/env python

import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import pandas as pd 
import seaborn as sns
sns.set(style="dark", color_codes=True)

def plot_categorical_data(dataframe, filename):

	## Plot
	ax = sns.stripplot(y=dataframe["Decrease"], x=dataframe["Program"], size=4)
	
	## Plot style
	plt.xticks(rotation=45)
	plt.gca().yaxis.set_major_formatter(mticker.FormatStrFormatter('%d %%'))
	plt.suptitle('Decrease in runtime over the baseline.', fontsize=17)
	plt.gcf().subplots_adjust(bottom=0.25)
	
	## axhlines
	cutoff_upper = plt.axhline(5, color='r', linestyle='--', label='Upper Suggested Cut Off Point.', linewidth=1)
	baseline = plt.axhline(0, color='b', linestyle='--', label='Median Baseline Runtime.', linewidth=1)
	cutoff_lower = plt.axhline(-5, color='r', linestyle='--', label='Lower Suggested Cut Off Point.', linewidth=1)
	
	## Legend
	legend = plt.legend(handles=[cutoff_upper, baseline, cutoff_lower], loc='upper left')
	
	## Save it
	sns.plt.savefig('/home/john/Thesis/ir/plots/' + filename + '.png')
	
	## Flush
	sns.plt.clf()
	sns.plt.cla()
	sns.plt.close()

	#plt.scatter(indicies, sorted_runtimes)
	#plt.gca().yaxis.set_major_formatter(mticker.FormatStrFormatter('%d %%'))
	#plt.ylabel('Increse over the baseline', fontsize=11)

	#plt.axhline(0, color='b', linestyle='--', label='Baseline')
	#plt.show()