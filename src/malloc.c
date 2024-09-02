#include <stddef.h>
#include <sys/mman.h>

void *malloc(size_t size) {
	(void)size;
	// void *ret = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	return NULL;
}
