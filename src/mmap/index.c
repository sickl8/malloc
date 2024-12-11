#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#ifndef __USE_MISC
#define __USE_MISC
#endif
#include <sys/mman.h>
#include "../index.h"

/*
 * return page size aligned length
 */
size_t align_to_page_size(size_t len) {
	int ps = getpagesize();
	size_t remainder = len % ps;
	// case remainder == 0: len += ps * 0 - 0 			<=> len += 0;
	// case remainder != 0: len += ps * 1 - remainer	<=> len += ps - remainder;
	len += ps * !!(remainder) - remainder;
	return len;
}

/*
 * Create MAPping with the default arguments for an anonymous mapping
 */
void *cmap(size_t *size) {
	size_t tmp = *size;
	*size = align_to_page_size(*size);
	// check for mmap fail or integer overflow due to align_to_page_size
	if (*size < tmp) {
		return MAP_FAILED;
	}
	global_tracker.mmap_calls++;
	global_tracker.total_real_size += *size;
	return mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

/*
 * Create MAPping with the default arguments for an anonymous mapping, supports a hint
 */
void *cmap_hint(size_t *size, void *hint) {
	size_t tmp = *size;
	*size = align_to_page_size(*size);
	// check for mmap fail or integer overflow due to align_to_page_size
	if (*size < tmp) {
		return MAP_FAILED;
	}
	global_tracker.mmap_calls++;
	return mmap(hint, *size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}