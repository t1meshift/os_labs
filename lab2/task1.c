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
  // Task 1
  printf("\n\nTask 1\n");
  int x = 100;
  FORK_TASK(
      {
        // `x' is copied from parent process
        assert(x == 100);
        x = 50;
      },
      {
        printf("Fork executed, pid: %d\n", f);
        x = 20;
        assert(x == 20);
        int kk = wait(NULL);
        printf("AFTER wait, pid: %d\n", kk);
        assert(x == 20);
      }
  )
  alarm(2);
  return 0;
}
