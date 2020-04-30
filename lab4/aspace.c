/* Example code for Think OS.

Copyright 2015 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/
#define PRINT_DISTANCE(a, b) printf("Distance between " #a " and " #b ": %ld\n", (void *) (a) - (void *) (b))

#include <malloc.h>
#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int global;
int another_global;

int main() {
  char malloc_trace_path[4096];
  getcwd(malloc_trace_path, 4096);
  strcat(malloc_trace_path, "/malloc_trace.txt");
  setenv("MALLOC_TRACE", malloc_trace_path, 0);
  mtrace();

  int local = 5;
  int another_local = 7;
  void *p = malloc(128);
  void *another_p = malloc((1 << 18));

  printf("Address of main is %p\n", main);
  printf("Address of global is %p\n", &global);
  printf("Address of local is %p\n", &local);
  printf("Address of another_local is %p\n", &another_local);
  printf("Address of p is %p\n", p);
  printf("Address of another_p is %p\n\n", another_p);

  PRINT_DISTANCE(&global, &main);
  PRINT_DISTANCE(&global, &another_global);
  PRINT_DISTANCE(&local, &another_local);
  PRINT_DISTANCE(p, another_p);

  malloc_stats();

  FILE *minfo = fopen("malloc_info.xml", "w");
  malloc_info(0, minfo);
  fclose(minfo);

  return 0;
}
