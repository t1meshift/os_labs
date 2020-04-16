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
  // Task 4
  printf("\n\nTask 4\n");
  FORK_TASK(
      {
        printf("Fork before wait\n");
        wait(NULL);
        printf("Fork AFTER wait\n");
        exit(228);
      },
      {
        printf("Parent before wait, pid of fork: %d\n", f);
        int status = -1488;
        int kk = waitpid(f, &status, 0);
        printf("pid %d, Execution status is %d\n", kk, WEXITSTATUS(status));
      }
  )

  return 0;
}
