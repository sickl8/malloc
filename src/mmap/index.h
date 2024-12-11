#ifndef __MMAP_PROTO_H__
#define __MMAP_PROTO_H__

#include <sys/mman.h>
#include <stddef.h>

size_t		align_to_page_size(size_t len);
void		*cmap(size_t *size);
void		*cmap_hint(size_t *size, void *hint);

#endif