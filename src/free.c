#include <stddef.h>
#include <sys/mman.h>
#include <pthread.h>

void free(void *ptr) {
	(void)ptr;
}
