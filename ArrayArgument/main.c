#include "stdlib.h"
#include "stdio.h"

my_fun_1(int fruit[2][3][5]) { ; }
my_fun_2(int (*fruit)[3][5]) { ; }
my_fun_3(int fruit[][3][5]) { ; }

int main() {

  int apricot[2][3][5];
  my_fun_1(apricot);
  my_fun_2(apricot);
  my_fun_3(apricot);

  int (*p)[3][5] = apricot;
  my_fun_1(p);
  my_fun_2(p);
  my_fun_3(p);

  int (*q)[2][3][5] = &apricot;
  my_fun_1(*q);
  my_fun_2(*q);
  my_fun_3(*q);

  return 0;

}
