#include "../../test.h"
#include "../../src/index.h"
#include "../../src/out.h"
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../../src/main/debug.h"

int	main(void)
{
	int		i;
	char	*addr;

	i = 0;
	while (i < 1024)
	{
		PRINT_MINFLT();
		addr = (char*)malloc(1024);
		PRINT_MINFLT();
		// size_t t = 0;
		// for (int i = 0; i < 1024; i++) {
		// 	assert(((char*)addr)[i] == 0);
		// }
		// print_hex((size_t)addr);
		// print_string("\n");
		// addr[0] = 42;
		// PRINT_MINFLT();
		i++;
	}
	print_string("test?\n");
	// pv(global_tracker.malloc_calls);
	// pv(global_tracker.mmap_calls);
	// pv(global_tracker.total_real_size);
	// pv(global_tracker.total_real_size / getpagesize());
	return (0);
}
