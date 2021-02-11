#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "setjmp.h"
#include "signal.h"

jmp_buf jmpSigSev;
struct sigaction sigActionSegv;

void sigSegvHandlerWithJmp(int signal, siginfo_t *si, void *arg) {
  printf("Caught segfault at address %p\n", si->si_addr);
  printf("regain control thanks to longjmp\n");
  longjmp(jmpSigSev, 1);
}

void sigSegvHandlerWithoutJmp(int signal, siginfo_t *si, void *arg) {
  printf("Caught segfault at address %p\n", si->si_addr);
  exit(1);
}

void sigSegvHandlerSetWithJmp(void) {
  memset(&sigActionSegv, 0, sizeof(struct sigaction));
  sigemptyset(&(sigActionSegv.sa_mask));
  sigActionSegv.sa_sigaction = sigSegvHandlerWithJmp;
  sigActionSegv.sa_flags = SA_SIGINFO;
  sigaction(SIGSEGV, &sigActionSegv, NULL);
}

void sigSegvHandlerSetWithoutJmp(void) {
  memset(&sigActionSegv, 0, sizeof(struct sigaction));
  sigemptyset(&(sigActionSegv.sa_mask));
  sigActionSegv.sa_sigaction = sigSegvHandlerWithoutJmp;
  sigActionSegv.sa_flags = SA_SIGINFO;
  sigaction(SIGSEGV, &sigActionSegv, NULL);
}

void withTryCatch() {
  int* ptr = NULL;
  // Set the hanlder for sig sev with longjmp
  sigSegvHandlerSetWithJmp();
  // Block protected from sig sev, equivalent to 'try'
  printf("Entering a block protected from sig sev\n");
  switch (setjmp(jmpSigSev)) {
    case 0:
      // Trigger a sig sev
      printf("Trigger sig sev\n");
      printf("%d",*ptr);
      break;
    // Equivalent to 'catch'
    case 1:
      printf("Sig sev has been caught\n");
      break;
  }
  printf("Exiting the block protected from sig sev\n");
}

void withoutTryCatch() {
  int* ptr = NULL;
  // Set the hanlder for sig sev without longjmp
  sigSegvHandlerSetWithoutJmp();
  printf("Entering a block not protected from sig sev\n");
  printf("Trigger sig sev\n");
  // Trigger a sig sev
  printf("%d",*ptr);
  // Never reach here
  printf("Exiting the block not protected from sig sev\n");
}

int main() {

  withTryCatch();
  withoutTryCatch();
  return 0;

}
