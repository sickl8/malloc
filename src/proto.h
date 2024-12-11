#ifndef __SRC_PROTO_H__
#define __SRC_PROTO_H__

#include "types.h"

int get_index_first_free_block(unsigned char *bf);
void set_block_availability(unsigned char *bf, int index, free_block_t unavailable);
free_block_t get_block_availability(unsigned char *bf, int index);
int zone_is_full(unsigned char *bf);
int zone_is_empty(unsigned char *bf);
void ft_memset(void *ptr, int c, size_t len);
char *output_number_base(size_t num, int base, char *output);
void print_number_base(size_t num, int base);
void print_hex(size_t num);
void print_string(char *str);
void print_number(size_t num);
int critical_assert();

#endif