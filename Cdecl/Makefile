main: Makefile main.o
	gcc -o main main.o

main.o: Makefile main.c
	gcc -c -Wall -Wextra main.c

test: test1 test2 test3 test4 test5 test6 test7 test8 test9
	

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

test6: main
	./main "char funA(int a, unsigned int b);"

test7: main
	./main "static const volatile float arr[2][NB];"

test8: main
	./main "char (*arr[2])(void);"

test9: main
	./main "int a[2]={2,3},*b=NULL;"

