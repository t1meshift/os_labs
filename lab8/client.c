#include "config.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define USAGE_STRING "Usage: %s [address [port]]\n" \
                     "`address' should be valid IPv4 address\n" \
                     "`port' should be in range [1, 65535]\n"
#define MAX(a,b) ((a)>(b)?(a):(b))
#define STR_EQ(a, b) (!strcmp((a),(b)))
#define WARNING(fmt, ...) fprintf(stderr,fmt,##__VA_ARGS__)
#define FATAL(fmt, ...) do {\
  WARNING(fmt,##__VA_ARGS__);\
  exit(1);\
} while (0)

int main(int argc, char *argv[]) {
  struct in_addr address = {.s_addr = inet_addr("127.0.0.1")};
  int port = PORT;

  switch (argc) {
    case 3:
      port = strtol(argv[2], NULL, 10);
      if (port <= 0 || port >= 0xFFFF) {
        FATAL("Could not parse port\n" USAGE_STRING, argv[0]);
      }
    case 2:
      if (!inet_aton(argv[1], &address)) {
        FATAL("Could not parse IPv4 address\n" USAGE_STRING, argv[0]);
      }
    case 1:
      break;
    default:
      FATAL(USAGE_STRING, argv[0]);
  }

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("socket()");
    FATAL("Error while creating socket\n");
  }
  struct sockaddr_in srv_addr = {
      .sin_family = AF_INET,
      .sin_addr = address,
      .sin_port = htons(port)
  };

  int err = connect(sockfd, (const struct sockaddr *) &srv_addr, sizeof(srv_addr));
  if (err == -1) {
    perror("connect()");
    FATAL("Connection error.\n");
  }

  char buff[4096];
  for (;;) {
    memset(buff, 0, 4096);
    printf("time - print current server time\n"
           "<filepath> - print contents of the file\n"
           "quit - quit client\n"
           "Enter command: ");
    int n = 0;
    while ((buff[n++] = getchar()) != '\n');
    buff[n - 1] = 0;

    // Some commands should be parsed on client side
    if (STR_EQ(buff, "quit")) {
      break;
    }

    if (send(sockfd, buff, n, 0) == -1) {
      perror("send()");
      FATAL("Could not send data.\n");
    }

    memset(buff, 0, 4096);
    if (recv(sockfd, buff, 4096, 0) == -1) {
      perror("recv()");
      FATAL("Receiving failed.\n");
    }
    printf("%s\n", buff);
  }

  close(sockfd);
  return 0;
}