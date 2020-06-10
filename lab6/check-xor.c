#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define USAGE_STRING "Usage: %s [-q] [-b] [--] [file [file...]]\n"
#define MAX(a,b) ((a)>(b)?(a):(b))
#define STR_EQ(a, b) (!strcmp((a),(b)))
#define FATAL(fmt, ...) do {\
  fprintf(stderr,fmt,##__VA_ARGS__);\
  exit(1);\
} while (0)
#define WARNING(fmt, ...) fprintf(stderr,fmt,##__VA_ARGS__)

bool flag_end = false;
bool quiet_mode = false;
bool benchmark = false;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    FATAL(USAGE_STRING, argv[0]);
  }

  for (int i = 1; i < argc; ++i) {
    if (!flag_end) {
      if (argv[i][0] != '-') {
        flag_end = true;
      }
      else if (STR_EQ(argv[i], "--")) {
        flag_end = true;
        continue;
      } else {
        switch (argv[i][1]) {
          case 'q':
            quiet_mode = true;
            break;
          case 'b':
            benchmark = true;
            break;
          default:
            FATAL(USAGE_STRING, argv[0]);
        }
        continue;
      }
    }
    FILE *f = fopen(argv[i], "rb");
    if (f == NULL) {
      perror("fopen()");
      FATAL("Could not open %s\n", argv[i]);
    }
    struct timeval t0, t1;
    int c;
    uint8_t s = 0;
    gettimeofday(&t0, NULL);
    while ((c = fgetc(f)) != EOF) {
      s ^= (uint8_t) c;
    }
    gettimeofday(&t1, NULL);

    if (quiet_mode) {
      printf("0x%02x\n", s);
    } else {
      printf("%s: checksum 0x%02x\n", argv[i], s);
    }
    if (benchmark) {
      struct timeval result;
      timersub(&t1, &t0, &result);
      printf("XOR execution time: %ld.%06lds\n", result.tv_sec, result.tv_usec);
    }
    fclose(f);
  }
}