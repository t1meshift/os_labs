#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

// A macro to simplify my life
#define FORK_TASK(IN_FORK, IN_THIS) { \
  int f = fork(); \
  switch (f) { \
   case -1: perror("fork() returned an error!"); exit(1); \
   case  0: IN_FORK; exit(0); \
   default: IN_THIS; \
  } \
}


int main() {
  // Task 2
  printf("\n\nTask 2\n");
  FORK_TASK(
      {
        printf("hello\n");
        // Continue parent process execution
        kill(getppid(), SIGCONT);
      },
      {
        printf("Fork executed, pid: %d\n", f);
        // Stop parent execution
        raise(SIGSTOP);
        printf("goodbye\n");
      }
  )

  return 0;
}
