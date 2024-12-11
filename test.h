#ifndef __TEST_H__
#define __TEST_H__

#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>

#define pv(x) { print_string(#x " = "); print_number(x); print_string("\n"); }
#define ps(x) { print_string(#x " = "); print_string(x); print_string("\n"); }
#define ok(x) { print_string("[\e[0;32m OK \e[0m] "); print_string(x); print_string("\n"); }
#define run_test(x) x(); ok(#x)

long get_seed() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * (long)1e6 + tv.tv_usec;
}

long seed;

#endif
