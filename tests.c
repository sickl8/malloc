#include <stdio.h>
#include "src/index.h"
#include <string.h>
#include "test.h"

void test_output_number_base() {
	size_t numbers[] = { 0b1110001010101UL, 0xABCDEF0123456789UL, 1234567890UL,  0,   0,   0,  (size_t)~0UL, (size_t)~0UL,  (size_t)~0UL         ,  14,   1,   10};
	char *strings[] = {   "1110001010101",   "ABCDEF0123456789", "1234567890" , "0", "0", "0", (char[65]){}, (char[17]){}, "18446744073709551615", "14", "1", "10"};
	int bases[] = {        2,                 16,                 10          ,  2,   16,  10, 2,            16,            10                   ,  10,   10,  10};
	ft_memset(strings[6], '1', 64);
	strings[6][64] = '\0';
	ft_memset(strings[7], 'F', 16);
	strings[7][16] = '\0';
	for (int i = 0; i < sizeof_array(numbers); i++) {
		char str[65];
		ft_memset(str, 0, 65);
		output_number_base(numbers[i], bases[i], str);
		assert(strcmp(str, strings[i]) == 0);
	}
}

void test_set_block_availability() {
	unsigned char bf[16];
	for (int i = 0; i < sizeof_array(bf); i++) {
		bf[i] = 255U;
	}
	for (int i = sizeof_array(bf) * 8 - 1; i > -1; i--) {
		set_block_availability(bf, i, FREE_BLOCK);
		assert(get_index_first_free_block(bf) == i);
	}
	for (int i = 0; i < sizeof_array(bf) * 8 - 1; i++) {
		set_block_availability(bf, i, USED_BLOCK);
		assert(get_index_first_free_block(bf) == i + 1);
	}
}

void test_get_index_first_free_block() {
	unsigned char bf[16];
	for (int j = 0; j < sizeof_array(bf) * 8; j++) {
		for (int i = 0; i < sizeof_array(bf); i++) {
			bf[i] = 255U;
			if (i == sizeof_array(bf) - 1) {
				bf[i]--;
			}
		}
		int index = j / 8;
		int bit_index = j % 8;
		bf[index] = (u8_t)(bf[index] ^ (u8_t)(0b10000000U >> bit_index));
		assert(get_index_first_free_block(bf) == j);
	}
}

int main() {
	run_test(test_get_index_first_free_block);
	run_test(test_set_block_availability);
	run_test(test_output_number_base);
}