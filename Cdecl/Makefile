main: Makefile main.o
	gcc -o main main.o

main.o: Makefile main.c
	gcc -c -Wall -Wextra main.c

test: test1 test2 test3 test4 test5
	

test1: main
	./main "char* const *(*declarator)();"

test2: main
	./main "struct banana* const *(*declarator)();"

test3: main
	./main "int declarator = 2;"

test4: main
	./main "struct {int a;} declarator;"

test5: main
	./main "char declaratorA, declaratorB;"

