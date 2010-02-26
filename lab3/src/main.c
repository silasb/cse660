#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
#include <stdlib.h>

#define BUFFER_SIZE 50

static char buffer[BUFFER_SIZE];

void handle_SIGINT()
{
  write(STDOUT_FILENO, buffer, strlen(buffer));
  exit(0);
}

int main()
{
  struct sigaction handler;
  handler.sa_handler = handle_SIGINT;
  sigaction(SIGINT, &handler, NULL);
  
  strcpy(buffer, "Caught <crtl><c>\n");
  
  while(1)
    ;
  return(0);
}
