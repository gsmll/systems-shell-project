#include "parse.h"
// executefunction:
// returns the function return value or errno on error or 0 on success
// params:  args [ pointer to args ]
// executes function in given args

int executefunction(char **args, int argc, int output, int input) {

  // tests for exit or cd
  if (args[0] == NULL)
    return -1;
  if (strcmp(args[0], "exit") == 0)
    exit(argc == 2 ? atoi(args[1]) : 0);
  if (strcmp(args[0], "cd") == 0) {
    if (argc == 2) {
      chdir(args[1]);
      return 0;
    } else {
      printf("too many args [cd]\n");
      return -1;
    }
  }

  // runs seperate programs
  int id = fork();

  if (id != 0) {

    int status;
    int w = wait(&status);
    if (w) {
      if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
      }
      if (WIFSIGNALED(status)) {
        return WTERMSIG(status);
      }
      return -1;
    } else {
      return errno;
    }
  }
  dup2(output, STDOUT_FILENO);
  dup2(input, STDIN_FILENO);
  execvp(args[0], args);
  return errno;
}
// runs execute for each function seperated by a semicolon, takes in stdin for
// now
void executeargs(char *input) {
  char *sect;
  char *search;
  search = strdup(input);
  while ((sect = strsep(&search, ";")) != NULL) {

    char argc;
    char **args = parse_args(strdup(sect), &argc);
    char *sect2;
    int output = 1;
    int fd = 0;
    char *sect3 = strdup(sect);
    //  printf("%s\n", sect);
    if ((sect2 = strsep(&sect, ">")) != NULL && sect != NULL) {
      printf("1");
      args = parse_args(sect2, &argc);
      while ((sect2 = strsep(&sect, " ")) != NULL && sect != NULL) {
      }
      output = open(sect2, O_WRONLY | O_CREAT | O_EXCL, 0666);
      if (output == -1) {
        remove(sect2);
        output = open(sect2, O_WRONLY | O_CREAT | O_EXCL, 0666);
        if (output == -1) {
          printf("Error [Reached \\n while parsing] (output) \n");
          return;
        }
      }
    }
    if ((sect2 = strsep(&sect3, "<")) != NULL && sect3 != NULL) {
      args = parse_args(sect2, &argc);
      while ((sect2 = strsep(&sect3, " ")) != NULL && sect3 != NULL) {
      }

      fd = open(sect2, O_RDONLY);
      if (fd == -1) {
        printf("Error [Reached \\n while parsing] (input) \n");
        return;
      }
    }
    executefunction(args, argc, output, fd);
  }
  return;
}
static void sighandler(int signo) {
  if (signo == SIGINT)
    printf(" - Interrupt - CTRL C\n");
}
int main() {
  signal(SIGINT, sighandler);
  while (1) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("[%s] shell@%s $ ", cwd, getpwuid(geteuid())->pw_name);
    char buffer[1024];
    fgets(buffer, 1024, stdin);
    buffer[strlen(buffer) - 1] = 0;
    executeargs(buffer);
  }
  return 0;
}
