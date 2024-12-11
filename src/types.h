#ifndef __SRC_TYPES_H__
#define __SRC_TYPES_H__

typedef unsigned char tiny_size_t;
typedef unsigned int small_size_t;

typedef enum	alloc_type_e {
	TINY_ALLOC = 0,
	SMALL_ALLOC = 1,
	LARGE_ALLOC = 2,
}				alloc_type_t;

typedef enum	free_block_e {
	FREE_BLOCK = 0,
	USED_BLOCK = 1,
}				free_block_t;

typedef alloc_type_t type_t;

typedef unsigned long u64_t;
typedef unsigned int u32_t;
typedef unsigned short u16_t;
typedef unsigned char u8_t;

#endif