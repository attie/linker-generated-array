all: main

run: main
	./main

main: lga.lds main.o
	gcc $(addprefix -Wl,,-T,$(filter %.lds,$^)) -o main $(filter %.o,$^)

main.o: main.c
	gcc -c $^ -o $@
