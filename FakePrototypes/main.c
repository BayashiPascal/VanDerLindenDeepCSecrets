#include "stdio.h"
#include "stdlib.h"

void banana_peel_1();
void banana_peel_2(char a, short b, float c);
void banana_peel_3(int a, float c, short b);
void banana_peel_4(char a, short b, float c);
void banana_peel_5(char a, float c, short b);
void banana_peel_6(char a, short b, float c);
int main() {

  printf("correct output is:\n");
  printf("char = a, short = 1, float = 2.0\n");
  printf("no prototype:\n");
  banana_peel_1('a', 1, 2.0);
  printf("correct prototype:\n");
  banana_peel_2('a', 1, 2.0);
  printf("wrong type in prototype in declaration:\n");
  banana_peel_3('a', 1, 2.0);
  printf("wrong type in prototype in definition:\n");
  banana_peel_4('a', 1, 2.0);
  printf("wrong order of arguments in prototype in declaration:\n");
  banana_peel_5('a', 2.0, 1);
  printf("wrong order of arguments in prototype in definition:\n");
  banana_peel_6('a', 1, 2.0);

  union {
    float f;
    double d;
  } u;
  u.f = 10.0;
  printf("set float, read double %f\n", u.d);
  u.d = 10.0;
  printf("set double, read float %f\n", u.f);

  return 0;

}
