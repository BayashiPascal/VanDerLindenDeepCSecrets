#include "stdio.h"
#include "stdlib.h"

int arr[1000]={1};
void funa(int* a) {
  printf("The stack top is now near %p\n", &a);
}
void funb(int* a, int* b) {
  printf("The stack top is now near %p, %p\n", &a, &b);
}
void func(int* a) {
  int* b;
  printf("The stack top is now near %p, %p\n", &a, &b);
}
void fund(int* a) {
  int* b=a;
  printf("The stack top is now near %p, %p\n", &a, &b);
}
int main() {

  int i;
  int *j = malloc(sizeof(int));
  printf("The data segment is near %p\n", &arr);
  printf("The text segment is near %p\n", &main);
  printf("The heap is near %p\n", j);
  printf("The stack top is near %p\n", &i);
  funa(j);
  funb(j,j);
  func(j);
  fund(j);
  return 0;

}
