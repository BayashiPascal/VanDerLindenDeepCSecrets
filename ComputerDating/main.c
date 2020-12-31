#include "time.h"
#include "stdlib.h"
#include "stdio.h"
#include "limits.h"
#include "unistd.h"
#include "assert.h"

int main() {

  // Referring to types.h and typesize.h
  // __STD_TYPE __TIME_T_TYPE __time_t;	/* Seconds since the Epoch.  */
  // and
  // #define __TIME_T_TYPE		__SYSCALL_SLONG_TYPE
  // and
  // #if defined __x86_64__ && defined __ILP32__
  // # define __SYSCALL_SLONG_TYPE	__SQUAD_TYPE
  // #else
  // # define __SYSCALL_SLONG_TYPE	__SLONGWORD_TYPE
  // and
  // #define __SLONGWORD_TYPE	long int
  // #if __WORDSIZE == 32
  // # define __SQUAD_TYPE		__quad_t
  // #elif __WORDSIZE == 64
  // # define __SQUAD_TYPE		long int
  // #else
  // # error
  // and
  // #if __WORDSIZE == 64
  // typedef long int __quad_t;
  // #else
  // __extension__ typedef long long int __quad_t;
  size_t sizeTimeT = sizeof(__SYSCALL_SLONG_TYPE);
  printf("time_t is a signed int of %zu byte(s)\n", sizeTimeT);

  // Referring to limits.h
  time_t timeMax = 0;
  if (sizeof(__SYSCALL_SLONG_TYPE) == sizeof(long int)) {

    timeMax = (__SYSCALL_SLONG_TYPE)LONG_MAX;

  } else if (sizeof(__SYSCALL_SLONG_TYPE) == sizeof(long long int)) {

    timeMax = (__SYSCALL_SLONG_TYPE)LLONG_MAX;

  } else {

    printf("Error, unexpected type for __SYSCALL_SLONG_TYPE\n");
    exit(1);

  }

  char* strTimeMax = ctime(&timeMax);
  printf("timeMax: %s", strTimeMax);

  time_t currentTime = time(NULL);
  printf("currentTime: %s", ctime(&currentTime));

  time_t remainTime = 1;
  while (remainTime > 0) {

    remainTime = difftime(timeMax, currentTime);
    long long int remainDays = remainTime / (60 * 60 * 24);
    long long int remainHours =
      (remainTime - remainDays * 24 * 60 * 60) / (60 * 60);
    long long int remainMinutes =
      (remainTime - remainDays * 24 * 60 * 60 - remainHours * 60 * 60) / 60;
    long long int remainSeconds =
      remainTime - remainDays * 24 * 60 * 60 - remainHours * 60 * 60 -
      remainMinutes * 60;

assert(timeMax == (currentTime + remainSeconds + remainMinutes * 60 + remainHours * 60 * 60 + remainDays * 60 * 60 * 24));
    printf(
      "remaining time: %lld days %lld hours %lld minutes %lld seconds      \r",
      remainDays, remainHours, remainMinutes, remainSeconds);
    fflush(stdout);
    sleep(1);
    currentTime = time(NULL);

  }

  printf("\nYou've been dogged by the daemon ! \n");

  return 0;

}
