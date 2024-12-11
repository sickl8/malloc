#include "test.h"
#include "src/mmap/index.h"

int main() {
	for (int i = 0; i < (1024 / 4); i++) {
		size_t size = getpagesize();
		char *ret = cmap(&size);
		for (int j = 0; j < 4; j++) {
			char *ptr = ret + 1024 * j;
			ptr[0] = 1;
		}
	}
}