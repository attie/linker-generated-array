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

/* this macro associates a variable with an ELF section and is not intended to be used by the user */
#define __lga_attrib(section_name) \
	__attribute__((unused, section(".lga_"#section_name)))

/* lga_el(type, array_name, element_name)
   lga_terminator(type, array_name, element_name)

   is used to declare a linker-generated array element
   this macro will declare the variable for you
   you can retrieve the specific element using lga_get_element() later

   lga_terminator() is designed for placing an element at the end of an array

   example usage:
     lga_el(type, numbers, special_number) = 12;
*/
#define lga_el(type, array_name, element_name) \
	type _lga_##array_name##_##element_name __lga_attrib(#array_name"_2_"#element_name)
#define lga_terminator(type, array_name, element_name) \
	type _lga_##array_name##_##element_name __lga_attrib(#array_name"_3_"#element_name)

/* _lga_el(array_name)
   _lga_terminator(array_name)

   is used to TAG a variable as a member of a linker-generated array
   this marco will not declare the variable for you
   you can't retrieve the specific element later

   _lga_terminator() is designed for placing an element at the end of an array

   example usage:
     int myInt _lga_el(numbers) = 10;
*/
#define _lga_el(array_name) \
	__lga_attrib(#array_name"_2")
#define _lga_terminator(array_name) \
	__lga_attrib(#array_name"_3")

/* lga_get_array(array_name)

   is used to get the first element of a linker-generated array
   this macro is for use within a function, as it is not valid at file-scope
   for file-scope access, see _lga_get_array()

   example usage:
     int *x = lga_get_array(numbers);
*/
#define lga_get_array(array_name) \
	({ \
		static char a[0] __lga_attrib(#array_name"_1"); \
		(void*)&a; \
	})

/* _lga_get_array(array_name)

   is used to declare an access variable for the array
   this macro should be used at flie-scope, as lga_get_array() is invalid there

   example usage:
     _lga_get_array(int, numbers, numberArray);

     void func(void) {
       printf("number[0]: %d\n", numberArray[0]);
     }
*/
#define _lga_get_array(type, array_name, variable_name) \
	static char __lga_##variable_name[0] __lga_attrib(#array_name"_1"); \
	type *variable_name = (type*)&__lga_##variable_name;
#define _lga_get_array_static(type, array_name, variable_name) \
	static char __lga_##variable_name[0] __lga_attrib(#array_name"_1"); \
	static type *variable_name = (type*)&__lga_##variable_name;

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
		static char a[0] __lga_attrib(#array_name"_4"); \
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
