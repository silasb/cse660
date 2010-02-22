#ifndef BUFFER_H
#define BUFFER_H

#include <time.h>

#define BUFFER_SIZE 5

typedef int buffer_t;

unsigned int junk;
buffer_t buffer[BUFFER_SIZE];

int insert_item(buffer_t);
int remove_item(buffer_t *);
void init_buffer();
void print_buffer();
void *producer(void *param);
void *consumer(void *param);

#endif // BUFFER_H
