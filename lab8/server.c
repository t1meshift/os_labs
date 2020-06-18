#include "config.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define STR_EQ(a, b) (!strcmp((a),(b)))
#define WARNING(fmt, ...) fprintf(stderr,fmt,##__VA_ARGS__)
#define FATAL(fmt, ...) do {\
  WARNING(fmt,##__VA_ARGS__);\
  exit(1);\
} while (0)

char *handle_cmd(char *buff) {
  char *ret = NULL;
  if (!*buff) {
    ret = strdup("Empty request\n");
  }
  else if (isalpha(buff[0])) {
    if (STR_EQ(buff, "time")) {
      // Respond with current time
      time_t t = time(NULL);
      ret = strdup(ctime(&t));
    } else {
      ret = strdup("Unknown command\n");
    }
  } else {
    // Get files
    int fd = open(buff, O_RDONLY);
    if (fd == -1) {
      perror("open()");
      WARNING("Can't open file %s\n", buff);
      return strdup("ERROR: server failed to obtain file.\n");
    }

    struct stat f_stat;
    if(fstat(fd, &f_stat) == -1) {
      perror("fstat()");
      WARNING("Can't stat file %s\n", buff);
      return strdup("ERROR: server failed to obtain file.\n");
    }
    ret = malloc(f_stat.st_size);
    read(fd, ret, f_stat.st_size);
    close(fd);
  }
  return ret;
}

void sig_hnd(int signum) {
  switch (signum) {
    case SIGUSR1:
      WARNING("SIGUSR1 handled\n");
      break;
    case SIGUSR2:
      WARNING("SIGUSR2 handled\n");
      break;
  }
}

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("socket()");
    FATAL("Error while creating socket\n");
  }
  fcntl(sockfd, F_SETFL, O_NONBLOCK);

  struct sockaddr_in srv_addr = {.sin_family = AF_INET,
                                 .sin_addr.s_addr = htonl(INADDR_ANY),
                                 .sin_port = htons(PORT)},
                     clt_addr;

  int err = bind(sockfd, (const struct sockaddr *) &srv_addr, sizeof(srv_addr));
  if (err == -1) {
    perror("bind()");
    FATAL("Error binding socket to address\n");
  }
  err = listen(sockfd, PENDING_CONNS_MAX);
  if (err == -1) {
    perror("listen()");
    FATAL("Error putting socket to passive mode\n");
  }
  printf("Listening on port %d...\n", PORT);

  signal(SIGUSR1, sig_hnd);
  signal(SIGUSR2, sig_hnd);

  char buff[4096] = {0};
  fd_set readset, allset;
  FD_ZERO(&allset);
  FD_SET(sockfd, &allset);
  struct timeval timeout = {.tv_sec = TIMEOUT_SECS, .tv_usec = 0};
  socklen_t socklen = sizeof(clt_addr);
  for (;;) {
    readset = allset;

    if (select(FD_SETSIZE, &readset, NULL, NULL, &timeout) == -1) {
      perror("select()");
      FATAL("Server error\n");
    }
    for (int i = 0; i < FD_SETSIZE; ++i) {
      if (FD_ISSET(i, &readset)) {
        if (i == sockfd) {
          int sock = accept(sockfd, (struct sockaddr *) &clt_addr, &socklen);
          if (sock == -1) {
            perror("accept()");
            FATAL("Acception failed.\n");
          }
          printf("Client accepted...\n");
          fcntl(sock, F_SETFL, O_NONBLOCK);
          FD_SET(sock, &allset);
        } else {
          int bytes_read = recv(i, buff, sizeof(buff), 0);
          if (bytes_read <= 0) {
            close(i);
            FD_CLR(i, &allset);
            printf("Client disconnected...\n");
            continue;
          }

          char *read_buff = handle_cmd(buff);

          if (send(i, read_buff, strlen(read_buff), 0) == -1) {
            perror("send()");
            FATAL("Error sending data.\n");
          }
          free(read_buff);
          memset(buff, 0, 4096);
        }
      }
    }
  }
  return 0;
}