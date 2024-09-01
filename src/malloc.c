#include <stddef.h>
#include <sys/mman.h>

void *malloc(size_t size) {
	(void)size;
	// void *ret = mmap(NULL, size, 0, 0, -1, 0);
	return NULL;
}
