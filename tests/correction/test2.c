#include "../../src/out.h"
#include "../../test.h"
#include "../../src/index.h"

int	main(void)
{
	// struct rusage usage;
	int		i;
	char	*addr;

	i = 0;
	// long before, after, afterr;
	// getrusage(RUSAGE_SELF, &usage);
	// before = usage.ru_minflt;
	// pv(before);
	// int mallooc = 0, deref = 0, freee = 0;
	while (i < 1024)
	{
		PRINT_MINFLT();
		addr = (char*)malloc(1024);
		// PRINT_MINFLT();
		// addr[0] = 42;
		// PRINT_MINFLT();
		// free(addr);
		// PRINT_MINFLT();
		i++;
	}
	return (0);
}
