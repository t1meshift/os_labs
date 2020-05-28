#include <assert.h>
#include <errno.h>
#include <fcntl.h>
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

/*
4. Разработать собственную версию (mystat) команды stat, которая просто осуществляет системный вызов stat(), выводит
размер, число задействованных блоков, число ссылок и т.д. Отследить, как меняется число ссылок, когда изменяется
содержимое директории.
 */

#define MAX(a,b) ((a)>(b)?(a):(b))
#define FATAL(fmt, ...) do {\
  fprintf(stderr,fmt,##__VA_ARGS__);\
  exit(1);\
} while (0)

int main(int argc, char *argv[]) {
  if (argc != 2) {
    FATAL("Usage: %s <path>\n", argv[0]);
  }

  char *path = argv[1];
  struct stat sb;

  if (lstat(path, &sb) == -1) {
    perror("lstat()");
    FATAL("Couldn't stat %s\n", path);
  }

  printf("File: %s\n", path);
  printf("Size: %ld\tBlocks: %ld\tIO Block: %ld\t", sb.st_size, sb.st_blocks, sb.st_blksize);
  switch(sb.st_mode & S_IFMT) {
    case S_IFBLK:
      puts("block device");
      break;
    case S_IFCHR:
      puts("character device");
      break;
    case S_IFDIR:
      puts("directory");
      break;
    case S_IFIFO:
      puts("FIFO/pipe");
      break;
    case S_IFLNK:
      puts("symlink");
      break;
    case S_IFREG:
      puts("regular file");
      break;
    case S_IFSOCK:
      puts("socket");
      break;
    default:
      puts("unknown?");
      break;
  }

  printf("Device: [%04x:%04x]\t", major(sb.st_dev), minor(sb.st_dev));
  printf("Inode: %lu\t", sb.st_ino);
  printf("Links: %lu\n", sb.st_nlink);

  printf("Mode: 0%o\tUid: %d\tGid: %d\n", sb.st_mode, sb.st_uid, sb.st_gid);
  printf("Access (atime): %s", ctime(&sb.st_atime));
  printf("Modify (mtime): %s", ctime(&sb.st_mtime));
  printf("Change (ctime): %s", ctime(&sb.st_ctime));
  return 0;
}