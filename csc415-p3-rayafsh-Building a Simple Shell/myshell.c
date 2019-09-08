/****************************************************************
 * Name        :   Raya Farshad                                             *
 * Class       : CSC 415                                        *
 * Date        :                                                *
 * Description :  Writting a simple bash shell program          *
 *                that will execute simple commands. The main   *
 *                goal of the assignment is working with        *
 *                fork, pipes and exec system calls.            *
 ****************************************************************/

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFERSIZE 256
#define PROMPT "myShell >> "
#define PROMPTSIZE sizeof(PROMPT)
#define MAX_TOKENS 4
#define WHITESPACE " \t\n\r\v\f"
//ssize_t getline(char **lineptr, size_t *n, FILE *stream);
//int execvp(const char *file, char *const argv[]);
//The exec family of functions, replaces the cirrent running process with a new process.
//It can be used to run a C program by using another C program. It comes under the header
//file unistd.h
//So by using execvp command, the created child process can run any program files which
//include a shell script.
//char *strtok(char*str, const char* delimiters)

//Global variables
char my_string[BUFFERSIZE];
char current_path[BUFFERSIZE];
char *parsed;
int bytes_read;
int exec_result;
int standard_output = 0;
int standard_output_append = 0;
int standard_input = 0;
int background = 0;
int is_pipe = 0;

char *ioFile;


//Read input and parse the tokens
char* read_parsed(){
fgets(my_string, BUFFERSIZE, stdin);
parsed = strtok(my_string, WHITESPACE);
 }

//Return the path
int cd(char* path){
   chdir(path);
  }

int run_command_pipe(char **arg, char** arg2){
  pid_t pid;
  pid_t pid2;
  //Iinitialize the fd input and output with
  int fd_in[2];
  pipe(fd_in);
  //starting the two process
  pid = fork();

  if(pid < 0){
    perror("Error while forking child process.\n");
  }else if(pid == 0){ // first child
    dup2(fd_in[0], 0);
    close(fd_in[1]);
    execvp(arg2[0], arg2);

  }else if((pid2 = fork()) == 0){ // second child
    dup2(fd_in[1], 1);
    close(fd_in[0]);
    execvp(arg[0], arg);
  }else{
      waitpid(pid2,NULL,0);
  }
}


int run_command_file1(char **arg, char* srcDestFile){
  pid_t pid;
  int i;
  int fd;
  pid = fork();

  if(pid < 0){
    perror("Error while forking child process.\n");
  }else if(pid == 0){
    // child process
    if(standard_output){
      fd = open(parsed, O_TRUNC | O_WRONLY | O_CREAT, 0666);

      dup2(fd ,1);
      close(fd);
      execvp(arg[0], arg);
      exit(1);
     }
    }else{
      waitpid(pid, 0, 0);
    }
  }
    int run_command_file2(char **arg, char* srcDestFile){
      pid_t pid;
      int i;
      int fd;
      pid = fork();

      if(pid < 0){
        perror("Error while forking child process.\n");
      }else if(pid == 0){
        // child process
        if(standard_output_append){
          fd = open(parsed,  O_APPEND | O_WRONLY | O_CREAT, 0666);
          dup2(fd ,1);
          close(fd);
          execvp(arg[0], arg);
          exit(1);
         }
        }else{
          waitpid(pid, 0, 0);
        }
      }
      int run_command_file3(char **arg, char* srcDestFile){
        pid_t pid;
        int fd;
        pid = fork();
        if(pid < 0){
          perror("Error while forking child process.\n");
        }else if(pid == 0){
          // child process
          if(standard_input){
            fd = open(parsed, O_RDONLY);
            dup2(fd, 0);
            close(fd);
            execvp(arg[0], arg);
            exit(1);
           }
          }else{
            waitpid(pid, 0, 0);
          }
        }



//main
int main(int* argc, char** argv)
{
  int myargc;
  char *myargv[BUFFERSIZE];
  char *myargv2[BUFFERSIZE];
  pid_t pid;
  int stat_loc;
  int fd;


  while (1) {
    getcwd(current_path, BUFFERSIZE);
    int myargc = 0;
    int background = 0;
    standard_output = 0;
    standard_output_append = 0;
    standard_input = 0;
    //print prompt " MYSHELL>>"
    printf(PROMPT);
    //read input from the user and parsed it
    read_parsed();

    while (parsed != NULL) {
      if(is_pipe){
        myargv2[myargc] = parsed;
      }else{
        myargv[myargc] = parsed;
      }
      myargc++;

    if (strcmp(myargv[0], "exit") == 0){
      printf("bye\n");
      return 0;
      }

    if (strcmp(myargv[0], "cd") == 0){
      cd(myargv[1]);
    }

    if (strcmp(myargv[0], "pwd") == 0){
      getcwd(current_path, BUFFERSIZE);
    }

    if (strcmp(myargv[myargc - 1], "&") == 0){
      myargv[myargc-1] = NULL;
      background = 1;
      printf("is background\n");
    }

    if(strcmp(parsed, ">") == 0){
      myargv[myargc - 1] = NULL;
      parsed = strtok(NULL, WHITESPACE);
      standard_output = 1;
      run_command_file1(myargv, parsed);
    }
    if(strcmp(parsed, ">>") == 0){
      myargv[myargc-1] = NULL;
      parsed = strtok(NULL, WHITESPACE);
      standard_output_append = 1;
      run_command_file2(myargv, parsed);
    }
    if(strcmp(parsed, "<") == 0){
      myargv[myargc-1] = NULL;
      parsed = strtok(NULL, WHITESPACE);
      standard_input = 1;
      run_command_file3(myargv, parsed);

    }
    if(strcmp(parsed, "|") == 0){
      myargv[myargc-1] = NULL;
      myargc = 0;

      is_pipe = 1;

    }

 parsed = strtok(NULL, WHITESPACE);
}

if(is_pipe){
  myargv2[myargc] = NULL;

  run_command_pipe(myargv, myargv2);
}else{
  myargv[myargc] = NULL;
  pid = fork();
  if(pid < 0){
    perror("Fork failed");
    exit(1);
  }
  if (pid == 0){
    exec_result = execvp(myargv[0], myargv);
    exit(1);
  }
  if (!background){
    while(wait(0) != pid);
  }else{
    waitpid(pid, &stat_loc, 0);
  }
}


 }
return 0;
}
