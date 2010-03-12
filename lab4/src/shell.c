/*
	Silas Baronda <barondas@cse.ohio-state.edu.edu>
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */

/* 
 * global variables 
 */
char *history_h[255][MAX_LINE/2+2];
int background_h[255];
int history = 0;

/*
 * structs 
 */
struct sigaction handler;

/* 
 * prototypes 
 */
int setup(char [], char *[],int *);
void exec_cmd(char *[], int);
int find_char(char);
void insert_history(char *[], int);
int open_history(FILE **, char *mode);
int read_history(FILE **);
void write_history(FILE **);
void handle_SIGINT();

/* 
 * functions 
 */
int 
main(void)
{
  handler.sa_handler = handle_SIGINT;
  sigaction(SIGINT, &handler, NULL);

  char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
  int background;             /* equals 1 if a command is followed by '&' */
  char *args[MAX_LINE/2+1];   /* command line (of 80) has max of 40 arguments */
  char *cwd;
  int last_cmd;

  FILE *history_fp;

  if(open_history(&history_fp, "r") == 1) {
    // if history file is present read from it.
    read_history(&history_fp);
  }

  while (1)
  {                  
    int return_s;
    background = 0;

    if((cwd = getcwd(NULL, 64)) == NULL) {
      perror("pwd");
      exit(2);
    }
    printf("%s%% ", cwd);

    fflush(0);

    if((return_s = setup(inputBuffer, args, &background)) == -1)       /* get next command */
      errno = 0;
    else if (return_s == 1) {
      if(open_history(&history_fp, "w") == 2) {
        write_history(&history_fp);
      }
      return 0;
    }
    else {
      if(strcmp(args[0], "history") == 0)
      {
        handle_SIGINT();
      }
      else if(strcmp(args[0], "r") == 0)
      {
        if(args[1] != NULL) {
          if((last_cmd = find_char(args[1][0])) == -1)
            printf("error: command starting with `%c' not found\n", args[1][0]);
          else
            background = background_h[last_cmd];
            insert_history(history_h[last_cmd], background);
            exec_cmd(history_h[last_cmd], background);
        }
        else {
          background = background_h[history-1];
          insert_history(history_h[history-1], background);
          exec_cmd(history_h[history-1], background);
        }
      }
      else if(strcmp(args[0], "exit") == 0) {
        if(open_history(&history_fp, "w") == 2) {
          write_history(&history_fp);
        }
        return 0;
      }
      else if(strcmp(args[0], "cd") == 0) {
        if(chdir(args[1]) == -1)
          perror("chdir");
      }
      else if(strcmp(args[0], "help") == 0)
        printf("Help is on the way!\n");
      else {
        insert_history(args, background);
        exec_cmd(args, background);
      }
    }
  }
}

/**
 * setup() reads in the next command line, separating it into distinct tokens
 * using whitespace as delimiters. setup() sets the args parameter as a 
 * null-terminated string.
 */
int 
setup(char inputBuffer[], char *args[],int *background)
{
  int length, /* # of characters in the command line */
      i,      /* loop index for accessing inputBuffer array */
      start,  /* index where beginning of next command parameter is */
      ct;     /* index of where to place the next parameter into args[] */

  ct = 0;

  /* read what the user enters on the command line */
  length = read(STDIN_FILENO, inputBuffer, MAX_LINE);  
  if(length == 1) return -1;

  if(errno == EINTR) return -1;

  start = -1;
  if (length == 0)
    return 1;            /* ^d was entered, end of user command stream */
  if (length < 0){
    perror("error reading the command");
    exit(-1);           /* terminate with error code of -1 */
  }

  /* examine every character in the inputBuffer */
  for (i = 0; i < length; i++) { 
    switch (inputBuffer[i]){
      case ' ':
      case '\t' :               /* argument separators */
        if(start != -1){
          args[ct] = &inputBuffer[start];    /* set up pointer */
          ct++;
        }
        inputBuffer[i] = '\0'; /* add a null char; make a C string */
        start = -1;
        break;

      case '\n':                 /* should be the final char examined */
        if (start != -1){
          args[ct] = &inputBuffer[start];     
          ct++;
        }
        inputBuffer[i] = '\0';
        args[ct] = NULL; /* no more arguments to this command */
        break;

      case '&':
        *background = 1;
        inputBuffer[i] = '\0';
        break;

      default :             /* some other character */
        if (start == -1)
          start = i;
    } 
  }    
  args[ct] = NULL; /* just in case the input line was > 80 */
  return 0;
} 

int 
find_char(char x)
{
  int temp = history - 1;
  for(;temp >= 0; temp--)
    if(history_h[temp][0][0] == x)
      return temp;
  return -1;
}

void 
exec_cmd(char *args[], int background)
{
  pid_t pid, w;

  pid = fork();

  if(pid >= 0)
  {
    if(pid == 0) // child
    {
      int return_code = execvp(args[0], args);
      if(return_code < 0)
        printf("%s: command not found\n", args[0]);
      _exit(return_code);
    }
    else // parent
    {
      if(background != 1) {
        do
        {
          w = waitpid(pid, NULL, 0);
        } while(w == -1 && errno == EINTR);
        if(w == -1)
        {
          printf("wait for child process failed!\n");
        }
      }
    }
  }
  else
  {
    fprintf(stderr, "Fork failed");
    exit(-1);
  }
}

void 
insert_history(char *args[], int background)
{
  int q = 0;
  for(;args[q] != NULL; q++)
    history_h[history][q] = strdup(args[q]);
  history_h[history][q] = NULL;
  background_h[history] = background;

  history++;
}

/* Will open the history file and return a file pointer to it
 * return 1 for successfully open with read
 * return 2 for successfully open with write
 * return 0 for history file not present
 * return -1 for error
 */
int
open_history(FILE **fp, char *mode)
{
  char *username;
  char *history_filename;

  if((username = getenv("USER")) == NULL) {
    fprintf(stderr, "can't get env %s\nPlease set your $USER var.\n", "USER");
    exit(0);
  }

  history_filename = strdup(username);

  strcat(history_filename, ".history");

  if((*fp = fopen(history_filename, mode)) != NULL) {
    if(strcmp(mode, "r") == 0) {
      return 1;
    }
    else if(strcmp(mode, "w") == 0) {
      return 2;
    }
  }
  else {
    if(strcmp(mode, "r") == 0) {
      return 0;
    }
  }

  return -1;
}

void 
parse_line(char *line, char *hello[], int *background)
{
  int i = 0;
  int start = 0;
  int count = 0;
  char temp[80];
  for(;i < (int)strlen(line); i++) {
    if(line[i] == ' ') {
      temp[start++] = '\0';
      hello[count] = strdup(temp);
      start = 0;
      count++;
    }
    else if(line[i] == '\n') {
      temp[start++] = '\0';
      hello[count] = strdup(temp);
      hello[count+1] = NULL;
    }
    else if(line[i] == '&')
      *background = 1;
    else {
      temp[start++] = line[i];
    }
  }
  hello[count+1] = NULL;
}

int 
read_history(FILE **fp)
{
  char buffer[MAX_LINE];
  int background;
  while(!feof(*fp)) {
    fgets(buffer, MAX_LINE + 1, *fp); // grabs newline
    if(feof(*fp))
      continue;
    char *hello[MAX_LINE/2+1];
    parse_line(buffer, hello, &background);
    insert_history(hello, background);
  }
  fclose(*fp);
  return 0;
}

void
write_history(FILE **fp)
{
  int i = 0;
  for(; history_h[i][0] != NULL; i++) {
    int j = 0;
    for(; history_h[i][j] != NULL; j++)
      fprintf(*fp, "%s ", history_h[i][j]);
    if(background_h[i] == 1)
      fprintf(*fp, "%c", '&');
    fprintf(*fp, "\n");
  }
  fclose(*fp);
}

void 
handle_SIGINT()
{
  printf("\nHistory\n");
  int temp = history-1;
  int subtracted = history - 10;
  if(subtracted <= 0)
    subtracted = 0;
  for(;temp >= subtracted; temp--) {
    printf("[%d] ", temp);
    int j = 0;
    for(; history_h[temp][j] != NULL; j++)
      printf("%s ", history_h[temp][j]);
    if(background_h[temp] == 1)
      printf("%c", '&');
    printf("\n");
  }
  handler.sa_handler = handle_SIGINT;
  sigaction(SIGINT, &handler, NULL);
}

