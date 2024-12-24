#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <sys/resource.h>
#include "../index.h"

static inline long get_minflt() {
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	return usage.ru_minflt;
}

static inline void print_minflt(char *file, int line) {
	long c = get_minflt();
	if (c != (long)global_tracker.minflt) {
		global_tracker.minflt = c;
		print_string(file);
		print_string(":");
		print_number(line);
		print_string(" ");
		pv(global_tracker.minflt);
	}
}

#endif
