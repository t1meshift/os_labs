#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

#define BENCHMARK(name, payload) \
  do {\
    struct timeval t0, t1;\
    gettimeofday(&t0, NULL);\
    payload \
    gettimeofday(&t1, NULL);\
    struct timeval result;\
    timersub(&t1, &t0, &result);\
    printf(name ": %ld.%06lds\n", result.tv_sec, result.tv_usec);\
  } while (0)

/*
1. Используя creat(), write(), fflush(), close(), gettimeofday(), разработать программу, которая
открывает файл,
записывает туда 300 KB,
очищает все записи,
закрывает и
удаляет файл,
а также измеряет и выводит время, затраченное на каждое действие.
*/

int main() {
  int f = -1;

  BENCHMARK("creat() call",{
    f = creat("task1.txt", S_IWUSR);
  });

  if (f == -1) {
    perror("creat()");
    return 1;
  }

  int bytes_written = -1;
  const size_t data_size = 307200; // bytes, 300KiB
  char *random_data = malloc(data_size);

  BENCHMARK("write 300KiB", {
    bytes_written = write(f, random_data, data_size);
  });
  free(random_data);
  
  if (bytes_written == -1) {
    perror("write()");
    return 1;
  }

  BENCHMARK("fsync() call", {
    fsync(f);
  });

  BENCHMARK("close() call", {
    close(f);
  });

  return 0;
}