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
  // Task 3
  printf("\n\nTask 3\n");
  FORK_TASK(
      {
        printf("Goodbye, fork, you are `ls' now\n");
        if (execlp("ls", "ls", "-lah", NULL) == -1) {
          perror("exec() error");
        };
      },
      {
        printf("Fork executed, pid: %d\n", f);
        //int status = -1488;
        int kk = wait(NULL);
        printf("AFTER wait, pid: %d\n", kk);
        //waitpid(f, &status, 0);
        //printf("Execution status is %d\n", status);
      }
  )

  return 0;
}
