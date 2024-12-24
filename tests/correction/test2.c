#include "../../src/out.h"
#include "../../test.h"
#include "../../src/index.h"
// #include "../../src/main/debug.h"
#include <sys/resource.h>

long get_minflt() {
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	return usage.ru_minflt;
}

void print_minflt(char *file, int line) {
	long c = get_minflt();
	if (c != (long)global_tracker.minflt) {
		global_tracker.minflt = c;
		print_string(file);
		print_string(":");
		print_number(line);
		print_string(" ");
		pv(global_tracker.minflt);
	}
}

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
	// PRINT_MINFLT();
	while (i < 1024)
	{
		addr = (char*)malloc(1024);
		// PRINT_MINFLT();
		addr[0] = 42;
		// PRINT_MINFLT();
		free(addr);
		i++;
	}
	// PRINT_MINFLT();
	return (0);
}
