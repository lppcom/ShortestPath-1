#include "Timer.h"

struct timeval start, end;

void set_clock(){
	gettimeofday(&start, NULL);
}

double elapsed_time(){
	gettimeofday(&end, NULL);

	double elapsed = (end.tv_sec - start.tv_sec);
	elapsed += (double)(end.tv_usec - start.tv_usec) / 1000000.0;
	return elapsed;
}
