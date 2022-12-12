#include "parse.h"
#include "main.h"


// executefunction:
// returns the function return value or errno on error or 0 on success
// params:  args [ pointer to args ]
// executes function in given args


int executefunction(char **args, int argc, int output, int input) {

  // tests for exit or cd
  if (args[0] == NULL) {
    free(args);
    return -1;
  }
  if (strcmp(args[0], "exit") == 0) {
    exit(argc == 2 ? atoi(args[1]) : 0);
  }
  if (strcmp(args[0], "cd") == 0) {
    if (argc == 2) {
      chdir(args[1]);
      free(args);
      return 0;
    } else {
      printf("too many args [cd]\n");
      free(args);
      return -1;
    }
  }

  // runs seperate programs

  int id = fork();

  if (id != 0) {

    int status;
    int w = waitpid(id, &status, 0);
    free(args);
    if (w) {
      if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
      }
      if (WIFSIGNALED(status)) {
        return WTERMSIG(status);
      }
      return -1;
    }
    return -1;
  }
  int backup_sdout = dup(STDOUT_FILENO); // save stdout for later
  int backup_sdin = dup(STDIN_FILENO);
  dup2(output, STDOUT_FILENO);
  dup2(input, STDIN_FILENO);
  execvp(args[0], args);
  dup2(backup_sdout, STDOUT_FILENO);
  dup2(backup_sdin, STDIN_FILENO);
  free(args);
  exit(-1);
}


// rdout:
// returns the filenumber for new output based on ">"
// params: command (current command),args (array of arguments from command line), argc( number of args)
// returns output fileno for output redirection and stdout in case of error


int rdout( char* command,char**args,char argc){
  char* input;
   if ((input = strsep(&command, ">")) != NULL && command != NULL) {
      memcpy(args, parse_args(input, &argc),(argc+1) * 8);
      while ((input = strsep(&command, " ")) != NULL && command != NULL && command[0] == ' ') {
      }
      command = strsep(&command, " ");
      int output = open(command, O_WRONLY | O_CREAT | O_EXCL, 0666);
      if (output == -1) {
        remove(command);
        output = open(command, O_WRONLY | O_CREAT, 0666);
        if (output == -1) {
          printf("Error [Reached \\n while parsing] (output) \n");
          return 1;
        }
      }
      return output;
    }
    return 1;
}


// rdin:
// returns the filenumber for new input based on "<" and pipes based on "|" (only one at a time)
// params: command (current command),args (array of arguments from command line), argc( number of args)
// returns input fileno for input redirection and stdin in case of error ( piping makes a file which is the new input)


int rdin( char* command, char**args,char argc){
    char* input;
char* command2 = strdup(command);
int fd = 0;
if ((input = strsep(&command, "|")) != NULL && command != NULL) {
      FILE *op = popen(input, "r");
      if (op == NULL) {
        printf("err");

        return 0;
      }
      fd = fileno(op);
      command = strsep(&command, ">");
      memcpy(args, parse_args(command, &argc),(argc+1) * 8);


    } else if ((input = strsep(&command2, "<")) != NULL && command2 != NULL) {
      memcpy(args, parse_args(input, &argc),(argc+1) * 8);

      command2 = strsep(&command2, ">");
      while ((input = strsep(&command2, " ")) != NULL && command2 != NULL &&
             command2[0] == ' ') {
      }
      if (command2 == NULL)
        command2 = input;
      command2 = strsep(&command2, " ");
      fd = open(command2, O_RDONLY);
      if (fd == -1) {
        printf("Error [Reached \\n while parsing] (input) \n");
        return 0;
      }
    }
    return fd;
}
// gatherargs
// runs execute for each function seperated by a semicolon
// input (string of input)
// returns an array of args for executings
void gatherargs(char *input) {
  char *sect;
  char *search;
  search = strdup(input);
  char buffer[100];
  while ((sect = strsep(&search, ";")) != NULL) {

    char argc;
    char **args = parse_args(strdup(sect), &argc);
    char *sect2;
    int output = 1;
    int fd = 0;
    char *sect3 = strdup(sect);
 

    output = rdout(sect,args,argc);
    fd = rdin(sect3,args,argc);
    executefunction(args, argc, output, fd);
  }
  return;
}
//catches control C and prints that an interrupt occured
static void sighandler(int signo) {
  if (signo == SIGINT)
    printf("\n - Interrupt - CTRL C\n");
}
// main
// repeatedly waits for command input and prints the home directory and the user
int main() {
  signal(SIGINT, sighandler);
  while (1) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf(" \033[0;34m[%s] shell@\033[1;31m%s \033[0m$ \033[1;33m", cwd,
           getpwuid(geteuid())->pw_name);
    char buffer[1024];
    fgets(buffer, 1024, stdin);
    buffer[strlen(buffer) - 1] = 0;
    printf("\033[0m");
    fflush(stdout);
    gatherargs(buffer);
  }
  return 0;
}
