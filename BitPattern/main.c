#include "stdlib.h"
#include "stdio.h"

int main() {

  int a = 0;
  float b = 0.0;
  
  if (sizeof(a) != sizeof(b)) {
    printf("a and b have different sizes\n");
    exit(0);
  }
  for (unsigned int i = 0; i < sizeof(a); ++i) {
    printf("%x %x\n",((char*)&a)[i],((char*)&b)[i]);
    if (((char*)&a)[i] ^ ((char*)&b)[i]) {
      printf("integer 0 and float 0.0 have different bit patterns\n");
      exit(0);
    }
  }
    printf("integer 0 and float 0.0 have same bit pattern\n");

  return 0;

}
