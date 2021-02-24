#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stropts.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

int val = 0;

void SigPollHandler(
  int signal,
  siginfo_t *si,
  void *arg);

void SetSigPollHandler(void) {

  struct sigaction sigActionSegv;
  memset(
    &sigActionSegv,
    0,
    sizeof(struct sigaction));
  sigemptyset(&(sigActionSegv.sa_mask));
  sigActionSegv.sa_sigaction = SigPollHandler;
  sigActionSegv.sa_flags = SA_SIGINFO;

  sigaction(
    SIGPOLL,
    &sigActionSegv,
    NULL);

}

void SigPollHandler(
  int signal,
  siginfo_t *si,
  void *arg) {

  char c = ' ';
  read(STDIN_FILENO, &c, 1);
  if (c == 'q') {
    exit(EXIT_SUCCESS);
  } else {
    val = 0;
  }

  SetSigPollHandler();

}

int main() {

  printf("tty %s\n",ttyname(fileno(stdin)));
  int fdStdin = open(ttyname(STDIN_FILENO), O_ASYNC);
  signal(SIGIO, handle_signal);
  fcntl(fdStdin, F_SETOWN, getpid());
  fcntl(fdStdin, F_SETFL, O_ASYNC | fcntl(STDIN_FILENO, F_GETFL));
  errno = 0;
  if (ioctl(fdStdin, I_SETSIG, S_RDNORM) < 0) {

    printf("ioctl failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);

  }

  while (1) {
    ++val;
    printf("%d\n", val);
    sleep(1);
  }

  return EXIT_SUCCESS;

}

// According to
// https://stackoverflow.com/questions/45373879/why-i-cant-receive-sigpoll-signal-from-ioctl-function
// The following should work, but it doesn't

//#include <stdio.h>
//#include <signal.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <fcntl.h>
//#include <ctype.h>

//void handle_signal(int a) { // Can only use async-signal safe functions here.
    //char msg[] = "got char c\n";
    //char* c = msg + (sizeof msg - 3);
    //if(1 != read(STDIN_FILENO, c, 1) || !isprint(*c))
        //return;
    //write(STDOUT_FILENO, msg, sizeof msg - 1);
    //if(*c == 'q')
        //exit(EXIT_SUCCESS);
//}

//int main() {
    //printf("type q to exit\n");

    //signal(SIGIO, handle_signal);
    //fcntl(STDIN_FILENO, F_SETFL, O_ASYNC | fcntl(STDIN_FILENO, F_GETFL));
    //fcntl(STDIN_FILENO, F_SETOWN, getpid());

    //sigset_t mask;
    //sigemptyset(&mask);
    //for(;;)
        //sigsuspend(&mask);

    //return EXIT_SUCCESS;
//}
