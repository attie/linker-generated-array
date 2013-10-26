#ifndef _LGA_H
#define _LGA_H

/* NOTE: these macros were inspired by those found in linker_lists.h of the U-Boot project */

/* WARNING: these macros are designed to be used in conjunction with a linker script
            the following basic script can be used, in conjunction with passing the
            appropriate linker script arguments: ld -T link.lds

        SECTIONS {
          __lga_start = .;
          .lga : {
            KEEP(*(SORT(.lga_*)));
          }
	        __lga_end = .;
        }
        INSERT BEFORE .bss;
*/

/* lga_el(type, array_name, element_name)

   is used to declare a linker-generated array element
   this macro will declare the variable for you, and you can later retrieve the specific element

   example usage:
     lga_el(type, numbers, special_number) = 12;
*/
#define lga_el(type, array_name, element_name) \
	type _lga_##array_name##_##element_name __attribute__((unused, section(".lga_"#array_name"_2_"#element_name)))

/* _lga_el(array_name)

   is used to TAG a variable as a member of a linker-generated array
   this marco will not declare the variable for you, and you can't retrieve a specific element later

   example usage:
     int myInt _lga_el(numbers) = 10;
*/
#define _lga_el(array_name) \
	__attribute__((unused, section(".lga_"#array_name"_2")))

/* lga_get_array(array_name)

   is used to get the first element of a linker-generated array

   example usage:
     int *x = lga_get_array(numbers);
*/
#define lga_get_array(array_name) \
	({ \
		static char a[0] __attribute__((unused, section(".lga_"#array_name"_1"))); \
		(void*)&a; \
	})

/* lga_get_element(type, array_name, element_name)

   is used to retrieve the address of a specific element
   the element must have been declared using lga_el()

   example usage:
     int *el = lga_get_element(int, numbers, special_number);
*/
#define lga_get_element(type, array_name, element_name) \
	({ \
		extern type _lga_##array_name##_##element_name; \
		(type *)&_lga_##array_name##_##element_name; \
	})

/* lga_get_array_end(type, array_name)

   is used to retrieve the address of the end of an array
   the pointer should not be dereferenced, but may be used for comparison

   example usage:
     int *el;
     for (el = lga_get_array(array); el < lga_get_array_end(numbers); el++) {
       ...
     }
*/
#define lga_get_array_end(array_name) \
	({ \
		static char a[0] __attribute__((unused, section(".lga_"#array_name"_3"))); \
		(void*)&a; \
	})

/* lga_get_array_size(array_name)

   is used to determine the size of an array IN BYTES

   example usage:
     size_t sz = lga_get_array_size(numbers);
*/
#define lga_get_array_size(array_name) \
	({ \
		void *s = lga_get_array(array_name); \
		void *e = lga_get_array_end(array_name); \
		size_t size = e - s; \
		size; \
	})

/* lga_get_array_lenght(type, array_name)

   is used to determine the number of elements in an array

   example usage:
     int count = lga_get_array_length(int, numbers);
*/
#define lga_get_array_length(type, array_name) \
	({ \
		size_t len = lga_get_array_size(array_name); \
		len /= sizeof(type); \
		len; \
	})

#endif /* _LGA_H */
