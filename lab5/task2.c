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
    struct timeval __t0, __t1;\
    gettimeofday(&__t0, NULL);\
    payload \
    gettimeofday(&__t1, NULL);\
    struct timeval __result;\
    timersub(&__t1, &__t0, &__result);\
    printf(name ": %ld.%06lds\n", __result.tv_sec, __result.tv_usec);\
  } while (0)

/*
2. Разработать программу, которая замеряет время для 300,000 однобайтовых записей с использованием
a) напрямую POSIX: creat(), write(), close().
b) с использованием библиотеки stdio (напр., fopen(), fwrite(), and fclose()).
Сравнить и объяснить результаты.
*/

int main() {
  BENCHMARK("POSIX API", {
    int f = -1;
    BENCHMARK("creat() call",{
      f = creat("task2_posix.txt", 0644);
    });

    if (f == -1) {
      perror("creat()");
      return 1;
    }

    int bytes_written = -1;
    const size_t data_size = 300000;
    char *random_data = malloc(data_size);

    BENCHMARK("write() call", {
      bytes_written = write(f, random_data, data_size);
    });
    free(random_data);

    if (bytes_written == -1) {
      perror("write()");
      return 1;
    }

    BENCHMARK("close() call", {
      close(f);
    });
  });

  BENCHMARK("stdio.h API",{
    FILE *f = NULL;
    BENCHMARK("fopen() call", {
      f = fopen("task2_stdio.txt", "w");
    });
    if (f == NULL) {
      perror("fopen()");
      return 1;
    }

    int bytes_written = -1;
    const size_t data_size = 300000;
    char *random_data = malloc(data_size);
    BENCHMARK("fwrite() call", {
      bytes_written = fwrite(random_data, 1, 300000, f);
    });
    free(random_data);
    if (bytes_written != data_size) {
      perror("fwrite()");
      return 1;
    }

    BENCHMARK("fclose() call",{
      fclose(f);
    });
  });

  return 0;
}