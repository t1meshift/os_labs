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
  // Task 8
  printf("\n\nTask 8\n");
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    perror("Couldn't create pipe!");
    exit(1);
  }
  char buff[256];

  FORK_TASK(
      {
        close(pipefd[0]);
        printf("In fork\n");
        dup2(pipefd[1], STDOUT_FILENO);
        snprintf(buff, 256, "Written from fork, pid %d\n", getpid());
        //printf("FORK: Message = %s, Len = %lu\n", buff, strlen(buff));
        write(pipefd[1], buff, strlen(buff));
        close(pipefd[1]);
      },
      {
        wait(NULL);
        close(pipefd[1]);
        printf("Fork responded:\n");
        while (read(pipefd[0], buff, 1) > 0) {
          printf("%c", buff[0]);
        }
        close(pipefd[0]);
      }
  )

  return 0;
}
