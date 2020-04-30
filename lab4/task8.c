#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

int main() {
  int *data = malloc(100 * sizeof(int));
  free(data);

  printf("%d\n", data[96]);

  return 0;
}