main: Makefile main.o
	gcc -o main main.o

main.o: Makefile main.c
	gcc -c -Wall -Wextra main.c

test: test1 test2 test3 test4
	

test1: main
	./main "char* const *(*next)();"

test2: main
	./main "struct banana* const *(*next)();"

test3: main
	./main "int a = 2;"

test4: main
	./main "struct {int a;} a;"
