#include <stddef.h>
#include <sys/mman.h>

void *realloc(void *ptr, size_t size) {
	(void)ptr;
	(void)size;
	return NULL;
}