#include <stdio.h>

int main() {
  char c;
  FILE *f = fopen("test.txt", "r"); // открыли файл
  if (!f) { // проверили успешность открытия
    printf("File not found.");
    return -1;
  }

  while (1) {
    fseek(f, 0, SEEK_SET);
    while ((c = fgetc(f)) !=
           EOF) { // считали символ, и проверили что он не конечный
      printf("%c", c);   // добавили XOR к ответу символа
    }
  }

  fclose(f);
  return 0;
}