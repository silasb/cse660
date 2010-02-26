/*
	Silas Baronda <barondas@cse.ohio-state.edu.edu>
	gcc -g shell.c -o mysh
  ./mysh
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */

void setup(char [], char *[],int *);

int main(void)
{
    char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
    int background;             /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE/2+1];   /* command line (of 80) has max of 40 arguments */
    char *cwd;
    
    /* Program terminates normally inside setup */
    while (1)
    {                  
      background = 0;
      if((cwd = getcwd(NULL, 64)) == NULL) {
        perror("pwd");
        exit(2);
      }

      printf("%s%% ", cwd);
      fflush(0);
      setup(inputBuffer, args, &background);       /* get next command */

      pid_t pid;

      pid = fork();

      if(pid >= 0)
      {
        if(pid == 0)
        {
          int return_code = execvp(args[0], args);
          if(return_code < 0)
            printf("%s: command not found\n", args[0]);
          _exit(return_code);
        }
        else
        {
          if(background == 0)
            wait(NULL);
            //waitpid(pid, NULL, NULL);
        }
      }
      else
      {
        fprintf(stderr, "Fork failed");
        exit(-1);
      }

      /* the steps are:
       (1) fork a child process using fork()
       (2) the child process will invoke execvp()
       (3) if background == 0, the parent will wait, 
        otherwise returns to the setup() function. */
    }
}

/**
 * setup() reads in the next command line, separating it into distinct tokens
 * using whitespace as delimiters. setup() sets the args parameter as a 
 * null-terminated string.
 */

void setup(char inputBuffer[], char *args[],int *background)
{
    int length, /* # of characters in the command line */
        i,      /* loop index for accessing inputBuffer array */
        start,  /* index where beginning of next command parameter is */
        ct;     /* index of where to place the next parameter into args[] */
    
    ct = 0;
    
    /* read what the user enters on the command line */
    length = read(STDIN_FILENO, inputBuffer, MAX_LINE);  

    start = -1;
    if (length == 0)
        exit(0);            /* ^d was entered, end of user command stream */
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
} 
