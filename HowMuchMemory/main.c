#include "stdlib.h"
#include "stdio.h"

int main() {

  long Mb = 0;
  while (malloc(1<<20)) ++Mb;
  printf("Allocated %ldMb total memory\n", Mb);

  return 0;

}
