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
  // Task 7 (whatever it means)
  printf("\n\nTask 7\n");
  FORK_TASK(
      {
        fclose(stdout);
        if (printf("Test\n") == -1) {
          perror("Task 7, printf() in fork error");
          fflush(stderr);
        }
      },
      {
        printf("Test1\n");
        int kk = wait(NULL);
        printf("AFTER wait, pid: %d\n", kk);
        printf("Test2\n");
      }
  )

  return 0;
}
