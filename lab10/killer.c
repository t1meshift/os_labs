#include <stdio.h>
#include <unistd.h>

int main() {
  if (unlink("test.txt") != 0) {
    printf("Unlink error.");
    return -1;
  }
  return 0;
}