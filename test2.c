#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
	int itr = 1000000;
	struct timeval start, end;
	gettimeofday(&start, NULL);

	for (int i = 0; i < itr; i++) {
		char *ptr = malloc(4096);
	}
	gettimeofday(&end, NULL);
	long time = (end.tv_sec * (long)1e6 + end.tv_usec) - (start.tv_sec * (long)1e6 + start.tv_usec);
	printf("took %lus%lums\n", time / (long)1e6, (time % (long)1e6) / 1000);
}