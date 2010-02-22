#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "buffer.h"

/* PROTOTYPES */
void print_help();

int main(int argc, char *argv[])
{
  if(argc <= 3) {
    print_help();
    exit(0);
  }
  int sleep_time = atoi(argv[1]);
  int n_producers = atoi(argv[2]);
  int n_consumers = atoi(argv[3]);

  init_buffer();

  print_buffer(buffer);

  int i = 0;
  for(;i<n_producers;i++) {
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, producer, NULL);
  }

  i = 0;
  for(;i<n_consumers;i++) {
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, consumer, NULL);
  }

  sleep(sleep_time);
  print_buffer(buffer);

  return 0;
}

void print_help()
{
  printf("Please provide\n\
      \t1. How long to sleep before terminating.\n\
      \t2. The number of producer threads.\n\
      \t3. The number of consumer threads.\n");
}
