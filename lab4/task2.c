#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void on_sigsegv() {
  printf("SIGSEGV\nWrite access to `char* a = \"abc\"` gives UB.\n");
  exit(0);
}

int main() {
  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_flags = SA_SIGINFO;
  action.sa_sigaction = on_sigsegv;
  sigaction(SIGSEGV, &action, NULL);

  /* Assume name shorter than 20 chars. */
  char name[20];
  puts("Enter name: ");
  scanf("%19s", name);
  printf("Hello,  %s.\n\n\tNice to see you.\n", name);

  puts("Enter name: ");
  scanf("%19s", name);
  printf("Hello,  %s.\n\n\tNice to see you.\n", name);

  char *name1 = "Anna";
  char a_letter = name1[0];
  name1[0] = name1[3];
  name1[3] = a_letter;
  puts(name1);

  return 0;
}