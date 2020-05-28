#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

/*
  3. Разработать собственную версию (mytail) команды tail.
  Формат: mytail -n file
  Она читает блок из конца файла, просматривает его с конца до заданного количества строк n и печатает эти строки
  в соответствующем порядке.
  Использовать: stat(), lseek(), open(), read(), close(), ...
 */
#define MAX(a,b) ((a)>(b)?(a):(b))
#define FATAL(fmt, ...) do {\
  fprintf(stderr,fmt,##__VA_ARGS__);\
  exit(1);\
} while (0)

int main(int argc, char *argv[]) {
  const int BUFF_SIZE = 4096;
  long n = 10;
  char *filename = NULL;

  // Parse arguments (skip 0th arg)
  for (int i = 1; i < argc; ++i) {
    if ((argv[i][0] == '-') && (i < argc - 1)) {
      // -n flag (e.g. -20)
      n = strtol(argv[i], NULL, 10) * -1;
      if (n < 1 || errno == ERANGE) {
        FATAL("Usage: %s [-<number_of_lines>] <file>\n", argv[0]);
      }
    } else {
      filename = argv[i];
      break;
    }
  }
  if (filename == NULL) FATAL("No file name specified.\n");

  struct stat sb;
  int fd;

  if (stat(filename, &sb) == -1) {
    perror("stat()");
    FATAL("Couldn't stat %s\n", filename);
  }
  if ((fd = open(filename, O_RDONLY)) == -1) {
    perror("open()");
    FATAL("Couldn't open %s\n", filename);
  }

  char *buf = calloc(BUFF_SIZE, 1);
  int lf_cnt = 0;
  size_t read_sz = 0;
  off_t file_offset = MAX(sb.st_size - BUFF_SIZE, 0);
  bool seek_stop = false;

  lseek(fd, -BUFF_SIZE, SEEK_END);

  do {
    int data_sz = read(fd, buf, BUFF_SIZE);
    for (int i = data_sz - 1; i >= 0; --i) {
      if (buf[i] == '\n') {
        if (++lf_cnt >= n) {
          file_offset += i + 1;
          goto print_lines;
        }
      }
    }
    file_offset -= BUFF_SIZE;
    lseek(fd, -BUFF_SIZE, SEEK_CUR);
  } while ((lf_cnt <= n) && (file_offset > 0));

print_lines:
  lseek(fd, file_offset, SEEK_SET);
  do {
    read_sz = read(fd, buf, BUFF_SIZE);
    write(STDOUT_FILENO, buf, read_sz);
  } while (read_sz == BUFF_SIZE);

  free(buf);
  close(fd);

  return 0;
}