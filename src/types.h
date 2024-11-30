#ifndef __SRC_TYPES_H__
#define __SRC_TYPES_H__

typedef unsigned char tiny_size_t;
typedef unsigned short small_size_t;

typedef enum	alloc_type_e {
	TINY_ALLOC = 0,
	SMALL_ALLOC = 1,
	LARGE_ALLOC = 2,
}				alloc_type_t;

typedef alloc_type_t type_t;

#endif