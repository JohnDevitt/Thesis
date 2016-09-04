

import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt
sns.set(style="darkgrid", color_codes=True)

#baseline = {'mergesort': 4.1700000000000008, 'nettle-arcfour': 0.49333333333333335, 'tarai': 0.026666666666666668, 'recursion': 0.046666666666666669, 'levenshtein': 1.9033333333333333, 'sglib-hashtable': 0.39999999999999997, 'bs': 0.0, 'ud': 0.096666666666666679}
 
#propositional = {'mergesort': 3.1833333333333336, 'nettle-arcfour': 0.47333333333333333, 'tarai': 0.049999999999999996, 'recursion': 0.05000000000000001, 'levenshtein': 1.5466666666666666, 'sglib-hashtable': 0.42999999999999999, 'bs': 0.0, 'ud': 0.12}
 
#relational = {'mergesort': 3.1633333333333336, 'nettle-arcfour': 0.46999999999999997, 'tarai': 0.01, 'recursion': 0.033333333333333333, 'levenshtein': 1.5566666666666666, 'sglib-hashtable': 0.41999999999999998, 'bs': 0.0, 'ud': 0.096666666666666679}

#############################33

baseline = {'sglib-dllist': 1.2633333333333334, 'newlib-mod': 0.0, 'cubic': 0.48999999999999999, 'rijndael': 20.59, 'fft': 0.0, 'qsort': 0.023333333333333334, 'ctl-string': 0.18000000000000002, 'sglib-hashtable': 0.46333333333333332, 'dtoa': 0.15333333333333332, 'statemate': 0.026666666666666668, 'nettle-cast128': 0.12333333333333334, 'minver': 0.029999999999999999, 'expint': 0.92000000000000004, 'crc32': 0.31, 'fac': 0.01, 'sglib-rbtree': 3.5, 'bubblesort': 2.0699999999999998, 'select': 0.096666666666666679}
 
forests = {'dtoa': 0.19666666666666668, 'expint': 0.90666666666666673, 'cubic': 0.49333333333333335, 'rijndael': 20.129999999999999, 'fft': 0.0, 'qsort': 0.02, 'ctl-string': 0.15666666666666665, 'sglib-hashtable': 0.41666666666666669, 'sglib-dllist': 1.1766666666666667, 'statemate': 0.02, 'nettle-cast128': 0.13, 'minver': 0.043333333333333335, 'newlib-mod': 0.0, 'crc32': 0.32666666666666666, 'fac': 0.01, 'sglib-rbtree': 3.5533333333333332, 'bubblesort': 2.0566666666666666, 'select': 0.10666666666666667}
 
neighbours = {'dtoa': 0.15333333333333332, 'expint': 0.90666666666666673, 'cubic': 0.3833333333333333, 'rijndael': 20.393333333333334, 'fft': 0.0, 'qsort': 0.02, 'ctl-string': 0.17666666666666667, 'sglib-hashtable': 0.59666666666666668, 'sglib-dllist': 1.2366666666666666, 'statemate': 0.026666666666666668, 'nettle-cast128': 0.13, 'minver': 0.033333333333333333, 'newlib-mod': 0.0033333333333333335, 'crc32': 0.29666666666666663, 'fac': 0.029999999999999999, 'sglib-rbtree': 3.5600000000000001, 'bubblesort': 1.5566666666666666, 'select': 0.099999999999999992}
 
tilde = {'sglib-dllist': 1.3, 'newlib-mod': 0.0, 'cubic': 0.41333333333333333, 'rijndael': 20.686666666666667, 'fft': 0.0, 'qsort': 0.023333333333333334, 'ctl-string': 0.17000000000000001, 'sglib-hashtable': 0.39333333333333331, 'dtoa': 0.15000000000000002, 'statemate': 0.02, 'nettle-cast128': 0.11666666666666665, 'minver': 0.01, 'expint': 0.92333333333333334, 'crc32': 0.31666666666666665, 'fac': 0.013333333333333334, 'sglib-rbtree': 3.5133333333333332, 'bubblesort': 2.0600000000000001, 'select': 0.033333333333333333}

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
ax.set_title('Performance of TILDE vs Random Forests')
xTickMarks = ['Group'+str(i) for i in range(1,6)]
ax.set_xticks(ind+width)
xtickNames = ax.set_xticklabels(keys)
plt.setp(xtickNames, rotation=45, fontsize=10)

## add a legend
ax.legend( (rects1[0], rects2[0], rects3[0]), ('Random Forests', 'Nearest Neighbours', 'Tilde') )

plt.show()