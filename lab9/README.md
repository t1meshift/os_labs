# Лабораторная работа №9

## Задание 2

Команда запуска:

`valgrind --tool=helgrind ./lab9_main-race_run`

Результат:

```text
...
==4862== Possible data race during read of size 4 at 0x30A014 by thread #1
==4862== Locks held: none
==4862==    at 0x108D27: main (main-race.c:15)
==4862==
==4862== This conflicts with a previous write of size 4 by thread #2
==4862== Locks held: none
==4862==    at 0x108CDF: worker (main-race.c:8)
==4862==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==4862==    by 0x4E496DA: start_thread (pthread_create.c:463)
==4862==    by 0x518288E: clone (clone.S:95)
==4862==  Address 0x30a014 is 0 bytes inside data symbol "balance"
...
```

helgrind сообщает, что возможно использование переменной balance в двух потоках параллельно.

### Решение проблемы:

Блокировать незащищенный процесс.

Заменить:
```c
balance++;
```

на:
```c
Pthread_mutex_lock(&m); // блокировка мьютекса
balance++;
Pthread_mutex_unlock(&m); // разблокировка
```


## Задание 3

Команда запуска:

`valgrind --tool=helgrind ./lab9_main-deadlock_run`

Результат:

```text
...
==6272== Thread #3: lock order "0x30A040 before 0x30A080" violated
==6272==
==6272== Observed (incorrect) order is: acquisition of lock at 0x30A080
==6272==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==6272==    by 0x108D06: worker (main-deadlock.c:13)
==6272==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x4E496DA: start_thread (pthread_create.c:463)
==6272==    by 0x518288E: clone (clone.S:95)
==6272==
==6272==  followed by a later acquisition of lock at 0x30A040
==6272==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==6272==    by 0x108D12: worker (main-deadlock.c:14)
==6272==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x4E496DA: start_thread (pthread_create.c:463)
==6272==    by 0x518288E: clone (clone.S:95)
==6272==
==6272== Required order was established by acquisition of lock at 0x30A040
==6272==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==6272==    by 0x108CEC: worker (main-deadlock.c:10)
==6272==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x4E496DA: start_thread (pthread_create.c:463)
==6272==    by 0x518288E: clone (clone.S:95)
==6272==
==6272==  followed by a later acquisition of lock at 0x30A080
==6272==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==6272==    by 0x108CF8: worker (main-deadlock.c:11)
==6272==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x4E496DA: start_thread (pthread_create.c:463)
==6272==    by 0x518288E: clone (clone.S:95)
==6272==
==6272==  Lock at 0x30A040 was first observed
==6272==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==6272==    by 0x108CEC: worker (main-deadlock.c:10)
==6272==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x4E496DA: start_thread (pthread_create.c:463)
==6272==    by 0x518288E: clone (clone.S:95)
==6272==  Address 0x30a040 is 0 bytes inside data symbol "m1"
==6272==
==6272==  Lock at 0x30A080 was first observed
==6272==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==6272==    by 0x108CF8: worker (main-deadlock.c:11)
==6272==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6272==    by 0x4E496DA: start_thread (pthread_create.c:463)
==6272==    by 0x518288E: clone (clone.S:95)
==6272==  Address 0x30a080 is 0 bytes inside data symbol "m2"
...
```

helgrind сообщает, что блокировка мьютексов происходит в неправильном порядке.

## Задание 4

Команда запуска:

`valgrind --tool=helgrind ./lab9_main-deadlock-global_run`

Результат:
```text
...
==7133== Thread #3: lock order "0x30A080 before 0x30A0C0" violated
==7133==
==7133== Observed (incorrect) order is: acquisition of lock at 0x30A0C0
==7133==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==7133==    by 0x108D12: worker (main-deadlock-global.c:15)
==7133==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x4E496DA: start_thread (pthread_create.c:463)
==7133==    by 0x518288E: clone (clone.S:95)
==7133==
==7133==  followed by a later acquisition of lock at 0x30A080
==7133==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==7133==    by 0x108D1E: worker (main-deadlock-global.c:16)
==7133==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x4E496DA: start_thread (pthread_create.c:463)
==7133==    by 0x518288E: clone (clone.S:95)
==7133==
==7133== Required order was established by acquisition of lock at 0x30A080
==7133==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==7133==    by 0x108CF8: worker (main-deadlock-global.c:12)
==7133==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x4E496DA: start_thread (pthread_create.c:463)
==7133==    by 0x518288E: clone (clone.S:95)
==7133==
==7133==  followed by a later acquisition of lock at 0x30A0C0
==7133==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==7133==    by 0x108D04: worker (main-deadlock-global.c:13)
==7133==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x4E496DA: start_thread (pthread_create.c:463)
==7133==    by 0x518288E: clone (clone.S:95)
==7133==
==7133==  Lock at 0x30A080 was first observed
==7133==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==7133==    by 0x108CF8: worker (main-deadlock-global.c:12)
==7133==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x4E496DA: start_thread (pthread_create.c:463)
==7133==    by 0x518288E: clone (clone.S:95)
==7133==  Address 0x30a080 is 0 bytes inside data symbol "m1"
==7133==
==7133==  Lock at 0x30A0C0 was first observed
==7133==    at 0x4C3403C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x108AD7: Pthread_mutex_lock (mythreads.h:23)
==7133==    by 0x108D04: worker (main-deadlock-global.c:13)
==7133==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7133==    by 0x4E496DA: start_thread (pthread_create.c:463)
==7133==    by 0x518288E: clone (clone.S:95)
==7133==  Address 0x30a0c0 is 0 bytes inside data symbol "m2"
...
```

helgrind сообщает об аналогичной ошибке 3 заданию, тогда как она отсутствует, так как дополнительный мьютекс не допускает такую ситуацию.

## Задание 5

Команда запуска:

`valgrind --tool=helgrind ./lab9_main-signal_run`

Результат:

```text
...
==7499== Possible data race during write of size 4 at 0x30A014 by thread #2
==7499== Locks held: none
==7499==    at 0x108D36: worker (main-signal.c:9)
==7499==    by 0x4C36C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==7499==    by 0x4E496DA: start_thread (pthread_create.c:463)
==7499==    by 0x518288E: clone (clone.S:95)
==7499==
==7499== This conflicts with a previous read of size 4 by thread #1
==7499== Locks held: none
==7499==    at 0x108D83: main (main-signal.c:16)
==7499==  Address 0x30a014 is 0 bytes inside data symbol "done"
...
```

helgrind сообщает о возможном одновременном использовании переменной done.

Такой метод ожидания потока не является эффективным, так как:

- может наступить чтение в момент записи
- требует дополнительных затрат ожидающего потока.

Наиболее эффективно было бы использовать мьютексы.

## Задание 6



Команда запуска:

`valgrind --tool=helgrind ./lab9_main-signal-cv_run`



Результат:

```text
...
==9132== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 7 from 7)
...
```

helgrind не удалось обнаружить ошибок.

По сравнению с прошлой программой данная более эффективна, так как она не производит постоянный опрос, 
а ожидает возвращение cond, и более безопасна для данных.
