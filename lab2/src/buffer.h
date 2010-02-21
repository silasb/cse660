#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE 5

typedef int buffer_t;

int insert_item(buffer_t *, buffer_t);
int remove_item(buffer_t *, buffer_t *);

#endif // BUFFER_H
