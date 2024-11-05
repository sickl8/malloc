#ifndef __ALLOC_LIB_H__
#define __ALLOC_LIB_H__

#include <stddef.h>

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void show_alloc_mem();

#endif /* __ALLOC_LIB_H__ */