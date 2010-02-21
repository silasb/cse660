#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"

void print_help();
void print_buffer(buffer_t *);

int main(int argc, char *argv[])
{
  if(argc <= 3) {
    print_help();
    exit(0);
  }
  int sleep = atoi(argv[1]);
  int n_producers = atoi(argv[2]);
  int n_consumers = atoi(argv[3]);

  buffer_t buffer[BUFFER_SIZE];
  buffer_t item = 2;
  insert_item(buffer, item);
  print_buffer(buffer);

  printf("%i, %i, %i\n", sleep, n_producers, n_consumers);
  return 0;
}

void print_help()
{
  printf("Please provide\n\
      \t1. How long to sleep before terminating.\n\
      \t2. The number of producer threads.\n\
      \t3. The number of consumer threads.\n");
}

void print_buffer(buffer_t *buffer)
{
  int i = 0;
  for(;i<=BUFFER_SIZE;i++)
    printf("buffer[%i] = %i\n", i, buffer[i]);
}
