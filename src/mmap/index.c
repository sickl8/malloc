#define __USE_MISC
#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../index.h"

#ifndef MAP_ANONYMOUS
	#define MAP_ANONYMOUS 0x20
#endif

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
 * memory mapping with a length that is a multiple of the system page-size
 */
inline void *psm_mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, off_t __offset) {
	return mmap(__addr, align_to_page_size(__len), __prot, __flags, __fd, __offset);
}

/*
 * allocate page size aligned size memory
 */
inline void *cmap(size_t size) {
	return psm_mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

/*
 * allocate page size aligned size memory 
 */
inline void *cmap_hint(size_t size, void *hint) {
	return psm_mmap(hint, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}