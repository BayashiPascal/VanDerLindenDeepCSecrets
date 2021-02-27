#include "stdlib.h"
#include "stdio.h"

char ga[] = "abcdefghijiklmnopqrstuvwxyz";

void fun_a(char ca[]) {

  printf("&ca:%p  &(ca[0]):%p  &(ca[1]):%p  \n", &ca, &(ca[0]), &(ca[1]));

}

void fun_b(char* pa) {

  printf("&pa:%p  &(pa[0]):%p  &(pa[1]):%p ++pa:%p \n",
    &pa, &(pa[0]), &(pa[1]), ++pa);

}

int main() {

  fun_a(ga);
  fun_b(ga);
  printf("&ga:%p  &(ga[0]):%p  &(ga[1]):%p  \n", &ga, &(ga[0]), &(ga[1]));
  return 0;

}
