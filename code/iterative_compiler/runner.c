
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


double timer(char *filepath) {

	clock_t start, end;
	double cpu_time_used;

	start = clock();
		system(filepath);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	return cpu_time_used;
}