#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifdef HANDLE_SIGSEGV
void on_sigsegv() {
  printf("SIGSEGV\nNull pointer dereference gives UB.\n");
  exit(0);
}
#endif

int main() {
#ifdef HANDLE_SIGSEGV
  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_flags = SA_SIGINFO;
  action.sa_sigaction = on_sigsegv;
  sigaction(SIGSEGV, &action, NULL);
#endif

  // Dereference NULL
  int *p = NULL;
  //(*p)(); // Calling a function from 0x0, yay!
  int a = *p;

  return 0;
}