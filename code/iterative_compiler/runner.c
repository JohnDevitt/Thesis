#include <time.h>
#include <system.h>

int main(int argc, char *argv[]) {

	filepath = argv[1]

	clock_t start, end;
	double cpu_time_used;

	start = clock();
		system(filepath);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	return cpu_time_used
}