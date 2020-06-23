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
} Queue;

Queue *make_queue(int length)
{
  Queue *queue = (Queue *) malloc(sizeof(Queue));
  queue->length = length;
  queue->array = (int *) malloc(length * sizeof(int));
  queue->next_in = 0;
  queue->next_out = 0;
  return queue;
}

int queue_incr(Queue *queue, int i)
{
  return (i+1) % queue->length;
}

int queue_empty(Queue *queue)
{
  // queue is empty if next_in and next_out are the same
  return (queue->next_in == queue->next_out);
}

int queue_full(Queue *queue)
{
  // queue is full if incrementing next_in lands on next_out
  return (queue_incr(queue, queue->next_in) == queue->next_out);
}

void queue_push(Queue *queue, int item) {
  if (queue_full(queue)) {
    perror_exit("queue is full");
  }
  
  queue->array[queue->next_in] = item;
  queue->next_in = queue_incr(queue, queue->next_in);
}

int queue_pop(Queue *queue) {
  if (queue_empty(queue)) {
    perror_exit("queue is empty");
  }
  
  int item = queue->array[queue->next_out];
  queue->next_out = queue_incr(queue, queue->next_out);
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
  for (i=0; i<QUEUE_LENGTH-1; i++) {
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

  for (i=0; i<QUEUE_LENGTH-1; i++) {
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
