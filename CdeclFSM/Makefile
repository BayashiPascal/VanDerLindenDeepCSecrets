main: Makefile main.o
	gcc -ggdb -o main main.o

main.o: Makefile main.c
	gcc -c -ggdb -Wall -Wextra main.c

test: test1
	

test1: main
	./main "char* const *(*next)(void);"

