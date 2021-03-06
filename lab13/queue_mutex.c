/* Example code for Think OS.

Copyright 2015 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include "utils.h"

#define NUM_CHILDREN 2
#define QUEUE_LENGTH 1600000

// QUEUE

typedef struct {
  int *array;
  int length;
  int next_in;
  int next_out;
  Mutex *mutex;
} Queue;

Queue *make_queue(int length)
{
  Queue *queue = (Queue *) malloc(sizeof(Queue));
  queue->length = length;
  queue->array = (int *) malloc(length * sizeof(int));
  queue->next_in = 0;
  queue->next_out = 0;
  queue->mutex = make_mutex();
  return queue;
}

int queue_incr(Queue *queue, int i)
{
  // NOTE: you must hold the mutex to call this function.
  return (i+1) % queue->length;
}

int queue_empty(Queue *queue)
{
  // NOTE: you must hold the mutex to call this function.
  // queue is empty if next_in and next_out are the same
  int res = (queue->next_in == queue->next_out);
  return res;
}

int queue_full(Queue *queue)
{
  // NOTE: you must hold the mutex to call this function.
  // queue is full if incrementing next_in lands on next_out
  int res = (queue_incr(queue, queue->next_in) == queue->next_out);
  return res;
}

void queue_push(Queue *queue, int item) {
  mutex_lock(queue->mutex);
  if (queue_full(queue)) {
    mutex_unlock(queue->mutex);
    perror_exit("queue is full");
  }
  
  queue->array[queue->next_in] = item;
  queue->next_in = queue_incr(queue, queue->next_in);
  mutex_unlock(queue->mutex);
}

int queue_pop(Queue *queue) {
  mutex_lock(queue->mutex);
  if (queue_empty(queue)) {
    mutex_unlock(queue->mutex);
    perror_exit("queue is empty");
  }
  
  int item = queue->array[queue->next_out];
  queue->next_out = queue_incr(queue, queue->next_out);
  mutex_unlock(queue->mutex);
  return item;
}

// SHARED

typedef struct {
  Queue *queue;
} Shared;

Shared *make_shared()
{
  Shared *shared = check_malloc(sizeof(Shared));
  shared->queue = make_queue(QUEUE_LENGTH);
  return shared;
}

// THREAD

pthread_t make_thread(void *(*entry)(void *), Shared *shared)
{
  int ret;
  pthread_t thread;

  ret = pthread_create(&thread, NULL, entry, (void *) shared);
  if (ret != 0) {
      perror_exit("pthread_create failed");
  }
  return thread;
}

void join_thread(pthread_t thread)
{
  int ret = pthread_join(thread, NULL);
  if (ret == -1) {
      perror_exit("pthread_join failed");
  }
}

// PRODUCER-CONSUMER

void *producer_entry(void *arg)
{
  int i;
  Shared *shared = (Shared *) arg;
  for (i=0; i<QUEUE_LENGTH; i++) {
    printf("adding item %d\n", i);
    queue_push(shared->queue, i);
  }
  pthread_exit(NULL);
}

void *consumer_entry(void *arg)
{
  int i;
  int item;
  Shared *shared = (Shared *) arg;

  for (i=0; i<QUEUE_LENGTH; i++) {
    item = queue_pop(shared->queue);
    printf("consuming item %d\n", item);
  }
  pthread_exit(NULL);
}

// TEST CODE

#include "testing.ic"

int main()
{
  thread_safe_test();
  empty_queue_test();
  return 0;
}
