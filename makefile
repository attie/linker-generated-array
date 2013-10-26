all: main

run: main
	./main

install: /usr/shared/lga/lga.lds /usr/include/lga.h
/usr/shared/lga/lga.lds: lga.lds
	install -g root -o root -DT -m 644 $^ $@
/usr/include/lga.h: lga.h
	install -g root -o root -DT -m 644 $^ $@
	

main: lga.lds main.o
	gcc $(addprefix -Wl,,-T,$(filter %.lds,$^)) -o main $(filter %.o,$^)

main.o: main.c
	gcc -c $^ -o $@
