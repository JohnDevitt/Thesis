
import App

baselines = []
forests = []
neighbours = []
relations = []

iterations = 10

for i in range(0, iterations):
	baseline, forest, neighbour, relational = App.main()
	baselines.append(baseline)
	forests.append(forest)
	neighbours.append(neighbour)
	relations.append(relational)

forsets_means = []
neighbours_means = []
relations_means = []

for iteration in range(0, iterations):
	
	baseline_count = 0
	forest_mean = 0
	neighbour_mean = 0
	relation_mean = 0

	for key in baselines[iteration]:

		base = baselines[iteration][key]
		if(not base == 0):
			baseline_count = baseline_count + 1
			forest_difference = ((base - forests[iteration][key])/base) * 100
			neighbour_difference = ((base - neighbours[iteration][key])/base) * 100
			relation_difference = ((base - relations[iteration][key])/base) * 100

			forest_mean = forest_mean + forest_difference
			neighbour_mean = neighbour_mean + neighbour_difference
			relation_mean = relation_mean + relation_difference

	forest_mean = forest_mean / baseline_count
	neighbour_mean = neighbour_mean / baseline_count
	relation_mean = relation_mean / baseline_count

	forsets_means.append(forest_mean)
	neighbours_means.append(neighbour_mean)
	relations_means.append(relation_mean)

print forsets_means

print "-----------"

print neighbours_means

print "-----------"

print relations_means