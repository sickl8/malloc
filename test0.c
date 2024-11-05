#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>

void *_malloc(size_t size) {
	void *ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (ret == ((void*)-1)) {
		return NULL;
	}
	return ret;
}

int main(int ac, char **av) {
	void *(*alloc)(size_t) = ac == 1 ? malloc : _malloc;
	int i;
	char *addr;
	i = 0;
	while (i < 1) {
		size_t s = (15UL << 30UL);
		addr = alloc(s + 1UL);
		// memset(addr, 42, 1024UL * 1024UL * 1024UL);
		printf("addr = %p, s = %zu\n", addr, s);
		for (size_t j = 0; j < s; j += 4096) {
			addr[j] = 42;
		}
		// free(addr);
		i++;
	}
	return (0);
}