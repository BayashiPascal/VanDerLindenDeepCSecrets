main: Makefile main.o
	gcc -o main main.o

main.o: Makefile main.c
	gcc -c -Wall -Wextra main.c

test1: main
	./main "char* const *(*next)();"

test2: main
	./main "struct banana* const *(*next)();"
