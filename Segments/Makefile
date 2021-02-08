main: Makefile main.o
	gcc -o main main.o

main.o: Makefile main.c
	gcc -c main.c

main_debug: Makefile main_debug.o
	gcc -ggdb -o main main.o

main_debug.o: Makefile main.c
	gcc -ggdb -c main.c

main_optim: Makefile main_optim.o
	gcc -O3 -o main main.o

main_optim.o: Makefile main.c
	gcc -O3 -c main.c

run: run1 run2 run3 run4 run5
	rm -f main.c main.o main

run1:
	echo '\
#include "stdio.h" \n\
int main() { \n\
  printf("hello world\\n"); \n\
  return 0; \n\
} \n\
' > main.c; make main; ls -l main main.o; size main main.o; make main_debug; ls -l main main.o; size main main.o; make main_optim; ls -l main main.o; size main main.o

run2:
	echo '\
#include "stdio.h" \n\
int arr[1000]; \n\
int main() { \n\
  printf("hello world\\n"); \n\
  return 0; \n\
} \n\
' > main.c; make main; ls -l main main.o; size main main.o; make main_debug; ls -l main main.o; size main main.o; make main_optim; ls -l main main.o; size main main.o

run3:
	echo '\
#include "stdio.h" \n\
int arr[1000]={1}; \n\
int main() { \n\
  printf("hello world\\n"); \n\
  return 0; \n\
} \n\
' > main.c; make main; ls -l main main.o; size main main.o; make main_debug; ls -l main main.o; size main main.o; make main_optim; ls -l main main.o; size main main.o

run4:
	echo '\
#include "stdio.h" \n\
void fun() { \n\
  int arr[1000]; \n\
} \n\
int main() { \n\
  printf("hello world\\n"); \n\
  return 0; \n\
} \n\
' > main.c; make main; ls -l main main.o; size main main.o; make main_debug; ls -l main main.o; size main main.o; make main_optim; ls -l main main.o; size main main.o

run5:
	echo '\
#include "stdio.h" \n\
void fun() { \n\
  int arr[1000]={1}; \n\
} \n\
int main() { \n\
  printf("hello world\\n"); \n\
  return 0; \n\
} \n\
' > main.c; make main; ls -l main main.o; size main main.o; make main_debug; ls -l main main.o; size main main.o; make main_optim; ls -l main main.o; size main main.o
