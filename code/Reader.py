

import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt
sns.set(style="darkgrid", color_codes=True)


# Returned from App.py
baseline = {'whetstone': 1.0566666666666666, 'stb_perlin': 1.0800000000000001, 'levenshtein': 1.8933333333333333, 'bubblesort': 2.063333333333333, 'sha': 98.773333333333326, 'sqrt': 6.3700000000000001, 'ndes': 1.0600000000000003, 'wikisort': 18.506666666666664, 'sglib-listsort': 1.0700000000000001}
 
forests = {'bubblesort': 2.0566666666666666, 'stb_perlin': 1.04, 'sqrt': 6.956666666666667, 'whetstone': 0.36999999999999994, 'sha': 81.366666666666674, 'levenshtein': 1.8999999999999997, 'ndes': 0.93666666666666665, 'wikisort': 22.493333333333336, 'sglib-listsort': 1.0766666666666667}
 
neighbours = {'bubblesort': 1.9533333333333331, 'stb_perlin': 0.94000000000000006, 'sqrt': 7.253333333333333, 'whetstone': 0.36999999999999994, 'sha': 84.140000000000001, 'levenshtein': 1.8633333333333333, 'ndes': 0.89000000000000001, 'wikisort': 15.18, 'sglib-listsort': 1.0433333333333332}
 
tilde = {'whetstone': 0.38000000000000006, 'stb_perlin': 0.65000000000000002, 'levenshtein': 1.5666666666666667, 'bubblesort': 1.5533333333333335, 'sha': 85.856666666666669, 'sqrt': 6.9500000000000002, 'ndes': 0.93333333333333324, 'wikisort': 14.656666666666666, 'sglib-listsort': 0.94666666666666666}



forest_speedups = {}
neighbours_speedups = {}
tilde_speedups = {}

for prog in baseline:
	base = baseline[prog]
	forest = forests[prog]
	neighbour = neighbours[prog]
	tild = tilde[prog]
	if(not base == 0):
		forest_speedups[prog] = ((base - forest)/base) * 100
		neighbours_speedups[prog] = ((base - neighbour)/base) * 100
		tilde_speedups[prog] = ((base - tild)/base) * 100



keys = [keys for keys in forest_speedups]


values_forests = []
values_neighbours = []
values_tilde = []

for key in keys:
	values_forests.append(forest_speedups[key])
	values_neighbours.append(neighbours_speedups[key])
	values_tilde.append(tilde_speedups[key])

all_vals = values_forests + values_neighbours + values_tilde

print sum(values_forests) / len(values_forests)
print sum(values_neighbours) / len(values_neighbours)
print sum(values_tilde) / len(values_tilde)

fig = plt.figure()
ax = fig.add_subplot(111)

## the data
N = len(keys)

## necessary variables
ind = np.arange(N)                # the x locations for the groups
width = 0.25                      # the width of the bars

## the bars
rects1 = ax.bar(ind, values_forests, width,
                color='black',
                error_kw=dict(elinewidth=2,ecolor='red'))

rects2 = ax.bar(ind+width, values_neighbours, width,
                    color='red',
                    error_kw=dict(elinewidth=2,ecolor='black'))

rects3 = ax.bar(ind+width+width, values_tilde, width,
                    color='green',
                    error_kw=dict(elinewidth=2,ecolor='black'))

# axes and labels
ax.set_xlim(-width,len(ind)+width)
ax.set_ylim(min(all_vals) - 10, max(all_vals) + 10)
ax.set_ylabel('Speed up over baseline(-O3)')
ax.set_title('Performance of TILDE vs Nearest NeighboursRandom Forests')
xTickMarks = ['Group'+str(i) for i in range(1,6)]
ax.set_xticks(ind+width)
xtickNames = ax.set_xticklabels(keys)
plt.setp(xtickNames, rotation=45, fontsize=10)

## add a legend
ax.legend( (rects1[0], rects2[0], rects3[0]), ('Random Forests', 'Nearest Neighbours', 'Tilde') )

plt.show()