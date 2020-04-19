#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  printf("Current PID: %d\n", getpid());

  const unsigned long mb_size = 1 << 20; // NOLINT: pow(2, 20) won't even touch the significant bit
  unsigned long alloc_size = 0;

  if (argc > 1) {
    alloc_size = atoi(argv[1]); // NOLINT: educational purpose only
  } else {
    scanf("%ld", &alloc_size); // NOLINT: educational purpose only
  }

  alloc_size *= mb_size;

  int *array = malloc(alloc_size);
  printf("Allocated %ldMb at %p\n", alloc_size / mb_size, array);

  srand(alloc_size);
  unsigned long arr_size = alloc_size / sizeof(*array);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
  // Intentional endless loop
  while (1) {
    for (int i = 0; i < arr_size; ++i) {
      array[i] = rand(); // NOLINT: randomness of rand() is unimportant in this case
    }
  }
#pragma clang diagnostic pop
  return 0;
}