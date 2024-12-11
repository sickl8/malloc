#include <stdlib.h>
#include <stdio.h>

int main() {
	for (int i = 0; i < 10000000; i++) {
		if (i % 1000 == 0) {
			printf("%d\n", i);
		}
		void *ptr = malloc(1024 * 1024);
	}
}