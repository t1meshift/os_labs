#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

/*
6. Разработать программу, которая выводит имена каждого файла и директории, начиная с заданной точки в дереве каталогов.
a) Без аргументов: сначала текущая директория и ее содержимое, затем поддиректории и т.д. (пока не закончится дерево,
root в качестве CWD).
b) С одним аргументом (который есть имя директории): все поддерево, начиная с заданной директории.
c) А также еще один какой-либо интересный вариант (см. опции для find).
 */

#define USAGE_STRING "Usage: %s [-d <max_depth>] [-L] [--] [path]\n"
#define MAX(a,b) ((a)>(b)?(a):(b))
#define STR_EQ(a, b) (!strcmp((a),(b)))
#define FATAL(fmt, ...) do {\
  fprintf(stderr,fmt,##__VA_ARGS__);\
  exit(1);\
} while (0)
#define WARNING(fmt, ...) fprintf(stderr,fmt,##__VA_ARGS__)

bool flag_end = false;
bool follow_symlinks = false;
long max_depth = LONG_MAX;

void traverse_path(char *path, long depth) {
  struct stat pstat;
  DIR *dir = NULL;
  struct dirent *ent;
  int stat_status;

  printf("%s\n", path);

  if (depth > max_depth)
    return;

  if (follow_symlinks) {
    stat_status = stat(path, &pstat);
  } else {
    stat_status = lstat(path, &pstat);
  }

  if (stat_status == -1) {
    perror("stat()");
    FATAL("Couldn't stat %s\n", path);
  }

  if (!S_ISDIR(pstat.st_mode))
    return;

  if ((dir = opendir(path)) == NULL) {
    perror("opendir()");
    FATAL("Couldn't open directory %s\n", path);
  }

  while ((ent = readdir(dir)) != NULL) {
    if (STR_EQ(ent->d_name, ".") || STR_EQ(ent->d_name, "..")) {
      continue;
    }

    char path_next[PATH_MAX];
    strcpy(path_next, path);

    if (path_next[strlen(path_next) - 1] != '/') {
      strcat(path_next, "/");
    }

    strcat(path_next, ent->d_name);

    traverse_path(path_next, depth + 1);
  }

  closedir(dir);
}

int main(int argc, char *argv[]) {
  char *path = ".";

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' && !flag_end) {
      char *flag = argv[i] + 1;
      if (STR_EQ(flag, "d")) {
        if (i + 1 < argc) {
          max_depth = strtol(argv[++i], NULL, 10);
          if (errno == ERANGE) {
            WARNING("Max depth level is out of range, %ld is set\n", max_depth);
            errno = 0;
          }
          if (max_depth < 0) {
            FATAL("Max depth must be non-negative\n");
          }
        } else {
          FATAL(USAGE_STRING, argv[0]);
        }
      }
      else if (STR_EQ(flag, "L")) {
        follow_symlinks = true;
      }
      else if (STR_EQ(flag, "-")) {
        flag_end = true;
      }
      else {
        FATAL(USAGE_STRING, argv[0]);
      }
    } else {
      path = argv[i];
      break;
    }
  }

  traverse_path(path, 1);
}