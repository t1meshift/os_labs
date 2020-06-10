#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crc8.h"

#define USAGE_STRING "Usage: %s <input_file> <checksum_file>\n"
#define MAX(a,b) ((a)>(b)?(a):(b))
#define STR_EQ(a, b) (!strcmp((a),(b)))
#define WARNING(fmt, ...) fprintf(stderr,fmt,##__VA_ARGS__)
#define FATAL(fmt, ...) do {\
  WARNING(fmt,##__VA_ARGS__);\
  exit(1);\
} while (0)

int main(int argc, char *argv[]) {
  if (argc != 3) {
    FATAL(USAGE_STRING, argv[0]);
  }

  FILE *f = fopen(argv[1], "rb");
  if (f == NULL) {
    perror("fopen()");
    FATAL("Could not open %s\n", argv[1]);
  }

  FILE *csfile = fopen(argv[2], "rb");
  if (csfile == NULL) {
    perror("fopen()");
    FATAL("Could not open %s\n", argv[2]);
  }

  int rsize = 0;
  uint8_t buf[4096] = {0};
  int blocks_read = 0;
  uint8_t s;
  uint8_t block_cs;

  while ((rsize = fread(buf, 1, 4096, f)) == 4096) {
    s = 0;
    for (int i = 0; i < 4096; ++i) {
      CRC8_MAXIM_ITER(s, buf[i]);
    }
    if (!fread(&block_cs, 1, 1, csfile)) {
      FATAL("Unexpected end of checksum file\n");
    }
    if (s != block_cs) {
      WARNING("Checksum mismatch at 4K-block #%d: got 0x%02x, expected 0x%02x\n", blocks_read, s, block_cs);
    }
    ++blocks_read;
  }
  if (rsize > 0) {
    s = 0;
    for (int i = 0; i < rsize; ++i) {
      CRC8_MAXIM_ITER(s, buf[i]);
    }
    if (!fread(&block_cs, 1, 1, csfile)) {
      FATAL("Unexpected end of checksum file\n");
    }
    if (s != block_cs) {
      WARNING("Checksum mismatch at 4K-block #%d: got 0x%02x, expected 0x%02x\n", blocks_read, s, block_cs);
    }
  }

  fclose(f);
  fclose(csfile);
}