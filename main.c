#include <stdio.h>
#include <stdlib.h>

#include "lga.h"

int unnamed _lga_el(numbers) = 8;

lga_el(int, numbers, five)       = 5;
lga_el(int, numbers, ten)        = 10;
lga_el(int, numbers, fifteen)    = 15;
lga_el(int, numbers, twenty)     = 20;
lga_el(int, numbers, twentyfive) = 25;

int main(int argc, char *argv[]) {
	int *numbers = lga_get_array(numbers);
	void *numbers_end = lga_get_array_end(numbers);
	size_t numbers_size = lga_get_array_size(numbers);
	size_t numbers_len = lga_get_array_length(int, numbers);

	int *num_fifteen = lga_get_element(int, numbers, fifteen);

	int i;

	printf("start: %p  end: %p  bytes: %d\n", numbers, numbers_end, numbers_end - (void*)numbers);
	printf("size: %d bytes -> %d elements\n", numbers_size, numbers_len);

	for (i = 0; i < numbers_len; i++) {
		printf("numbers[%d]: %d\n", i, numbers[i]);
	}

	printf("fifteen: %d\n", *num_fifteen);

	return 0;
}
