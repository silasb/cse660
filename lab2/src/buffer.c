#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t empty, full;
pthread_mutex_t mutex;

int in = 0;
int out = 0;

int insert_item(buffer_t item)
{
  if(sem_wait(&empty) != 0)
    return -1;
  if(pthread_mutex_lock(&mutex) != 0)
    return -1;

  buffer[in % BUFFER_SIZE] = item;
  printf("producer produced %d at %d\n", item, in % BUFFER_SIZE);
  in++;

  if(pthread_mutex_unlock(&mutex) != 0)
    return -1;
  if(sem_post(&full) != 0)
    return -1;

  return 0;
}

int remove_item(buffer_t *item)
{
  if(sem_wait(&full) != 0)
    return -1;
  if(pthread_mutex_lock(&mutex) != 0)
    return -1;

  *item = buffer[out % BUFFER_SIZE];
  printf("consumer consumed %d at %d\n", *item, out % BUFFER_SIZE);
  out++;

  if(pthread_mutex_unlock(&mutex) != 0)
    return -1;
  if(sem_post(&empty) != 0)
    return -1;

  return 0;
}

void init_buffer()
{
  int i = 0;
  for(;i<BUFFER_SIZE;i++)
    buffer[i] = 0;

  sem_init(&full, 0, 0);
  sem_init(&empty, 0, BUFFER_SIZE);
  pthread_mutex_init(&mutex, NULL);
}

void print_buffer()
{
  int i = 0;
  for(;i<BUFFER_SIZE;i++)
    printf("buffer[%i] = %i\n", i, buffer[i]);
}

void *producer(void *param)
{
  buffer_t item;
  param = NULL;
  while(1)
  {
    //usleep(item);

    item = rand_r(&junk); // assign a random value

    if(insert_item(item) < 0)
      printf("error\n");

  }
}

void *consumer(void *param)
{
  buffer_t item;
  param = NULL;
  while(1)
  {
    //usleep(item);

    if(remove_item(&item) < 0)
      printf("error removing\n");

    printf("consume_item(item) => %d\n", item);
  }
}
