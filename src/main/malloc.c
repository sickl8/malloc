#define __USE_MISC
#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../index.h"

void *malloc(size_t size) {
	if	(!size) {
		return NULL;
	}
	if (size > SMALL_BLOCK_SIZE) {
		// large allocation
		
	}
	void *ret = NULL;
	return ret;
}
