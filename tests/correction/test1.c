// #include "../../test.h"
// #include "../../src/index.h"
// #include "../../src/out.h"
#include <stdlib.h>
#include <assert.h>

int	main(void)
{
	int		i;
	char	*addr;

	i = 0;
	while (i < 1024)
	{
		addr = (char*)malloc(1024);
		size_t t = 0;
		for (int i = 0; i < 1024; i++) {
			assert(((char*)addr)[i] == 0);
		}
		// addr[0] = 42;
		i++;
	}
	// pv(global_tracker.malloc_calls);
	// pv(global_tracker.mmap_calls);
	// pv(global_tracker.total_real_size);
	// pv(global_tracker.total_real_size / getpagesize());
	return (0);
}
