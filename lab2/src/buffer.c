#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>

int insert_item(buffer_t *buffer, buffer_t item)
{
  buffer[item] = item;
  printf("producer produced %d\n", item);
  return 0;
}

int remove_item(buffer_t *buffer, buffer_t *item)
{
  printf("consumer consumed %d\n", 1);
  return 0;
}
