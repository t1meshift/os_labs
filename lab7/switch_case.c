#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define IS_ALPHA(x) (((x) >= 'A' && (x) <= 'Z') || ((x) >= 'a' && (x) <= 'z'))

typedef union {
  uint64_t i64;
  char     chr[8];
} str_block;

#define CHUNK_SIZE (sizeof(str_block))

int chunk_switch_case(str_block *chunk) {
  size_t i = 0;
  for (; i < CHUNK_SIZE; ++i) {
    if (!chunk->chr[i]) {
      break;
    }
    if (IS_ALPHA(chunk->chr[i])) {
      chunk->chr[i] ^= 0x20;
    }
  }
  return i;
}

char *str_switch_case(const char *str) {
  size_t str_size = strlen(str);
  size_t chunked_read_border = str_size - (str_size%CHUNK_SIZE);
  char *result = malloc(str_size);
  str_block chunk = {0};

  for (size_t i = 0; i < chunked_read_border; i += CHUNK_SIZE) {
    memcpy(chunk.chr,str + i, CHUNK_SIZE);
    chunk_switch_case(&chunk);
    memcpy(result + i, chunk.chr, CHUNK_SIZE);
  }
  for (size_t i = chunked_read_border; i < str_size; ++i) {
    result[i] = str[i];
    if (IS_ALPHA(result[i])) {
      result[i] ^= 0x20;
    }
  }
  return result;
}

int main(){
  str_block chunk = {0};
  char stop_reading = 0;

  do {
    if (isatty(STDIN_FILENO)) {
      for (size_t i = 0; i < CHUNK_SIZE; ++i) {
        fread(&chunk.chr[i], 1, 1, stdin);
        if (chunk.chr[i] == '\n') {
          chunk.chr[i] = 0;
          break;
        }
      }
    } else {
      fread(&chunk.i64, 1, CHUNK_SIZE, stdin);
    }
    stop_reading = (chunk_switch_case(&chunk) != CHUNK_SIZE);
    fwrite(&chunk.chr, 1, CHUNK_SIZE, stdout);
    chunk.i64 = 0;
  } while (!stop_reading);

  return 0;
}