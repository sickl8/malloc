#ifndef __MMAP_PROTO_H__
#define __MMAP_PROTO_H__

#include <sys/mman.h>
#include <stddef.h>

size_t		align_to_page_size(size_t len);
inline void	*psm_mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, off_t __offset);
inline void	*cmap(size_t size);
inline void	*cmap_hint(size_t size, void *hint);

#endif