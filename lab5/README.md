# Лабораторная работа №5

## Задание 1

Результат выполнения:
```text
creat() call: 0.000350s
write 300KiB: 0.000346s
fsync() call: 0.005649s
close() call: 0.000011s
```

## Задание 2

Результат выполнения:
```text
creat() call: 0.000262s
write() call: 0.000275s
close() call: 0.000357s
POSIX API: 0.001043s
fopen() call: 0.000206s
fwrite() call: 0.000258s
fclose() call: 0.000455s
stdio.h API: 0.000968s
```

При многократном выполнении время сильно разнится, порой не в пользу POSIX API. Для повышения 
точности таймера следует значительно увеличить размер файла.

## Задание 3

Была реализована упрощённая версия команды `tail`. По умолчанию она читает последние 10 строк,
но это поведение можно изменить параметром `-n`, где `n` -- количество строк (например, `-24`)

## Задание 4

Была реализована упрощённая версия команды `stat`. Пример вывода для пути `/dev/null`:
```text
File: /dev/null
Size: 0 Blocks: 0       IO Block: 4096  character device
Device: [0000:0006]     Inode: 5        Links: 1
Mode: 020666    Uid: 0  Gid: 0
Access (atime): Tue Jun  9 20:25:43 2020
Modify (mtime): Tue Jun  9 20:25:43 2020
Change (ctime): Tue Jun  9 20:25:43 2020
```

Для `/home`:
```text
File: /home
Size: 10        Blocks: 0       IO Block: 4096  directory
Device: [0000:001d]     Inode: 264      Links: 1
Mode: 040755    Uid: 0  Gid: 0
Access (atime): Fri Apr  5 22:15:25 2019
Modify (mtime): Mon Dec  4 18:26:29 2017
Change (ctime): Fri Sep  7 03:19:26 2018
```

При изменении содержимого директории число ссылок не изменяется.

## Задание 5

Была реализована упрощённая версия команды `ls`.

Были реализованы следующие ключи:

- `-l` для подробного вывода (long-listing) 
- `--` для явного завершения приёма параметров (на случай, если файл начинается с дефиса)

Пример для параметров `-l -- /usr`:
```text
drwxr-xr-x         1    root root          104  2020-Jun-09 03:13:52 +10        .
drwxr-xr-x         1    root root          254  2020-May-22 07:15:49 +10        ..
drwxr-xr-x         1    root root       127286  2020-Jun-09 03:13:51 +10        bin
drwxr-xr-x         1    root root        35922  2020-Jun-09 03:13:51 +10        include
drwxr-xr-x         1    root root       287352  2020-Jun-09 03:13:51 +10        lib
drwxr-xr-x         1    root root        45012  2020-Jun-08 12:29:17 +10        lib32
drwxr-xr-x         1    root root           72  2017-Oct-26 22:22:02 +10        local
drwxr-xr-x         1    root root         7954  2020-Jun-08 13:31:41 +10        share
drwxr-xr-x         1    root root           70  2020-May-26 13:59:54 +10        src
drwxr-xr-x         1    root root            6  2019-Dec-17 13:10:37 +10        games
drwxr-xr-x         1    root root           40  2020-Apr-06 17:00:36 +10        libexec
lrwxrwxrwx         1    root root            3  2020-May-21 17:30:57 +10        lib64
lrwxrwxrwx         1    root root            3  2020-May-21 17:30:57 +10        sbin
```

Пример для параметров `/bin/sh`:
```text
/bin/sh
```

Пример для `-l /bin/sh`:
```text
lrwxrwxrwx         1    root root            4  2020-May-27 05:32:57 +10        /bin/sh
```

## Задание 6

Была реализована утилита, напоминающая упрощённую версию `find`.

Были реализованы следующие ключи:

- `-d <number>` для ограничения глубины рекурсивного прохода по директориям 
    (`<number>` -- неотрицательное)
- `-L` для перехода по символическим ссылкам (по умолчанию директории-симлинки игнорируются)
- `--` для явного завершения приёма параметров (на случай, если файл начинается с дефиса)

Примеры:
```text
$ ./lab5_task6.c_run -L -d 2 ../lab5                                                                       ✔ 
../lab5
../lab5/CMakeFiles
../lab5/CMakeFiles/lab5_task1.c_run.dir
../lab5/CMakeFiles/lab5.dir
../lab5/CMakeFiles/CMakeDirectoryInformation.cmake
../lab5/CMakeFiles/lab5_benchmark.h_run.dir
../lab5/CMakeFiles/lab5_task2.c_run.dir
../lab5/CMakeFiles/lab5_mytail.c_run.dir
../lab5/CMakeFiles/lab5_mystat.c_run.dir
../lab5/CMakeFiles/lab5_myls.c_run.dir
../lab5/CMakeFiles/lab5_task6.c_run.dir
../lab5/CMakeFiles/progress.marks
../lab5/cmake_install.cmake
../lab5/task2_posix.txt
../lab5/task2_stdio.txt
../lab5/lab5_mytail.c_run
../lab5/lab5_myls.c_run
../lab5/Makefile
../lab5/lab5.cbp
../lab5/lab5_task1.c_run
../lab5/task1.txt
../lab5/lab5_task2.c_run
../lab5/lab5_mystat.c_run
../lab5/lab5_task6.c_run
```

```text
$ ./lab5_task6.c_run -d 0
.
```

```text
$ ./lab5_task6.c_run -L -d 1
.
./CMakeFiles
./cmake_install.cmake
./task2_posix.txt
./task2_stdio.txt
./lab5_mytail.c_run
./lab5_myls.c_run
./Makefile
./lab5.cbp
./lab5_task1.c_run
./task1.txt
./lab5_task2.c_run
./lab5_mystat.c_run
./lab5_task6.c_run
```