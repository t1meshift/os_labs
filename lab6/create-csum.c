#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crc8.h"

#define USAGE_STRING "Usage: %s <input_file> <output_file>\n"
#define MAX(a,b) ((a)>(b)?(a):(b))
#define STR_EQ(a, b) (!strcmp((a),(b)))
#define FATAL(fmt, ...) do {\
  fprintf(stderr,fmt,##__VA_ARGS__);\
  exit(1);\
} while (0)
#define WARNING(fmt, ...) fprintf(stderr,fmt,##__VA_ARGS__)

int main(int argc, char *argv[]) {
  if (argc != 3) {
    FATAL(USAGE_STRING, argv[0]);
  }

  FILE *f = fopen(argv[1], "rb");
  if (f == NULL) {
    perror("fopen()");
    FATAL("Could not open %s\n", argv[1]);
  }

  FILE *fw = fopen(argv[2], "wb");
  if (fw == NULL) {
    perror("fopen()");
    FATAL("Could not open %s\n", argv[2]);
  }

  int rsize = 0;
  uint8_t buf[4096] = {0};
  uint8_t s;

  while ((rsize = fread(buf, 1, 4096, f)) == 4096) {
    s = 0;
    for (int i = 0; i < 4096; ++i) {
      CRC8_MAXIM_ITER(s, buf[i]);
    }
    fwrite(&s, 1, 1, fw);
  }
  if (rsize > 0) {
    s = 0;
    for (int i = 0; i < rsize; ++i) {
      CRC8_MAXIM_ITER(s, buf[i]);
    }
    fwrite(&s, 1, 1, fw);
  }

  fclose(f);
  fclose(fw);
}