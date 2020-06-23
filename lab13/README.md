# Лабораторная работа №13

Для тестирования трёх вариантов реализации очереди были реализованы следующие тесты:

1. `empty_queue_test()` для теста проблемы producer-consumer. Если программа некорректно
работает при наличии потоков-потребителей, она выдаст ошибку при попытке взять элемент из
пустой очереди.

2. `thread_safe_test()` для проверки корректности работы многопоточности. Идея заключается
в заполнении очереди ненулевыми значениями из нескольких потоков. Если программа работает некорректно на нескольких
потоках, то программа выдаст ошибку. Ошибка определяется в наличии нулей между ненулевыми значениями,
которые могут оказаться там ввиду гонок данных, например, в `queue_push()`.

Тесты лежат в отдельном файле [testing.ic](./testing.ic), этот файл инклудится в каждой из программ.

Результаты предсказуемы: 

- наивная реализация не проходит мультипоточный тест (producer-consumer, 
очевидно, тоже);
- реализация на мьютексах корректно работает с несколькими потоками, но проблема producer-consumer
остаётся;
- реализация с использованием условных переменных проходит все тесты.

В наивной реализации проблемы начинаются в `queue_push()`. 
В этом участке кода происходит data race, а так как `next_in` ничем не защищён, следовательно, 
двойной инкремент неизбежен при достаточно большом размере очереди.
```c
queue->array[queue->next_in] = item;
queue->next_in = queue_incr(queue, queue->next_in);
``` 

Для решения проблемы producer-consumer была добавлена conditional variable для 
проверки на пустоту очереди, тем самым поток-потребитель дождётся, пока в очереди
не появится хотя бы один элемент.