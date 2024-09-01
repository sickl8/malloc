#ifndef __ALLOC_LIB_H__
#define __ALLOC_LIB_H__

#include <stdlib.h>

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

#endif /* __ALLOC_LIB_H__ */