#include <assert.h>
#include <dirent.h>
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
#include <pwd.h>
#include <grp.h>

/*
5. Разработать собственную версию (myls) команды ls, которая выводит список файлов в заданной директории.
С ключом -l она выводит информацию о каждом файле, включая собственника, группу, разрешения и т.д., получаемые из системного
вызова stat().
Формат: myls -l directory (или текущую директорию, если параметр не задан)
Использовать: stat(), opendir(), readdir(), getcwd(), ...
 */

#define MAX(a,b) ((a)>(b)?(a):(b))
#define STR_EQ(a, b) (!strcmp((a),(b)))
#define FATAL(fmt, ...) do {\
  fprintf(stderr,fmt,##__VA_ARGS__);\
  exit(1);\
} while (0)

char *path = NULL;
bool long_listing = false;
bool flag_end = false;

void cleanup_on_exit() {
  free(path);
}

void print_entry(char *entry) {
  if (long_listing) {
    struct stat fstat;
    if (lstat(entry, &fstat) == -1) {
      perror("stat()");
      FATAL("Couldn't stat %s\n", entry);
    }

    // Print permissions
    char entry_type;
    // setuid, setgid, sticky are not in count
    const char* perm_sym[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    int perm = fstat.st_mode & 0777;
    struct passwd *pw_data;
    struct group *gp_data;

    char user_name[33] = "";
    char group_name[33] = "";
    char last_mtime[256] = "";

    switch (fstat.st_mode & S_IFMT) {
      case S_IFREG:
        entry_type = '-';
        break;
      case S_IFDIR:
        entry_type = 'd';
        break;
      case S_IFLNK:
        entry_type = 'l';
        break;
      case S_IFBLK:
        entry_type = 'b';
        break;
      case S_IFSOCK:
        entry_type = 's';
        break;
      case S_IFCHR:
        entry_type = 'c';
        break;
#ifdef S_IFIFO
      case S_IFIFO:
        entry_type = 'p';
        break;
#endif /* S_IFIFO */
      default:
        entry_type = '?';
        break;
    }

    if ((pw_data = getpwuid(fstat.st_uid)) == NULL) {
      perror("Could not get owner username");
      errno = 0;
      snprintf(user_name, 33, "%d", fstat.st_uid);
    } else {
      snprintf(user_name, 33, "%s", pw_data->pw_name);
    }

    if ((gp_data = getgrgid(fstat.st_gid)) == NULL) {
      perror("Could not get owner group name");
      errno = 0;
      snprintf(group_name, 33, "%d", fstat.st_gid);
    } else {
      snprintf(group_name, 33, "%s", gp_data->gr_name);
    }

    time_t mtime = fstat.st_mtime;
    struct tm* tm_info = localtime(&mtime);
    strftime(last_mtime, 256, "%Y-%b-%d %T %Z", tm_info);

    printf("%c%s%s%s\t%4lu\t%s %s\t%6ld\t%s\t",
           entry_type,
           perm_sym[(perm & S_IRWXU) >> 6],
           perm_sym[(perm & S_IRWXG) >> 3],
           perm_sym[perm & S_IRWXO],
           fstat.st_nlink,
           user_name,
           group_name,
           fstat.st_size,
           last_mtime);

  }
  printf("%s\n", entry);
}

int main(int argc, char *argv[]) {
  atexit(cleanup_on_exit);

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' && !flag_end) {
      char *flag = argv[i] + 1;
      if (STR_EQ(flag, "l")) {
        long_listing = true;
      }
      else if (STR_EQ(flag, "-")) {
        flag_end = true;
      }
      else {
        FATAL("Usage: %s [-l] [--] [path]\n", argv[0]);
      }
    } else {
      path = strdup(argv[i]);
      break;
    }
  }

  if (!path) {
    /*
     * As an extension to the POSIX.1-2001 standard, glibc's getcwd()
     * allocates the buffer dynamically using malloc(3) if buf is NULL.  In
     * this case, the allocated buffer has the length size unless size is
     * zero, when buf is allocated as big as necessary.  The caller should
     * free(3) the returned buffer.
     */
    if ((path = getcwd(NULL, 0)) == NULL) {
      perror("getcwd()");
      FATAL("Couldn't get current working directory\n");
    }
  }

  struct stat path_stat;
  DIR *dir = NULL;
  if (stat(path, &path_stat) == -1) {
    perror("stat()");
    FATAL("Couldn't stat %s\n", path);
  }

  bool is_dir = S_ISDIR(path_stat.st_mode);

  if (is_dir) {
    if ((dir = opendir(path)) == NULL) {
      perror("opendir()");
      FATAL("Couldn't open directory %s\n", path);
    }
    if (chdir(path) == -1) {
      perror("chdir()");
      FATAL("Could not chdir to directory %s\n", path);
    }

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
      print_entry(ent->d_name);
    }
    closedir(dir);
  } else {
    print_entry(path);
  }

  return 0;
}