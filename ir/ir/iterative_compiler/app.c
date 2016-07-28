
#include <stdio.h>
#include <getCPUtime.c>

int main() {	
	double startTime, endTime;

	startTime = getCPUTime( );
		printf("test");
	endTime = getCPUTime( );

	fprintf( stderr, "CPU time used = %lf\n", (endTime - startTime) );
}