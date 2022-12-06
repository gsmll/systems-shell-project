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
  char buffer[100];
  char argc;
  char **args = parse_args(search, &argc);
  int start = 0;
  int end = argc;
  int output;
  int fd;
  for (int i = 0; i < argc; i++) {
    if (strcmp(args[i], ">") == 0) {
      if (i + 1 >= argc) {
        printf("Error [Reached \\n while parsing] (output) \n");
        return;
      }
      output = open(args[i + 1], O_WRONLY | O_CREAT | O_EXCL, 0666);
      if (output == -1) {
        remove(args[i + 1]);
        output = open(args[i + 1], O_WRONLY | O_CREAT | O_EXCL, 0666);
        if (output == -1) {
          printf("Error [Reached \\n while parsing] (output) \n");
          return;
        }
      }
      if (end > i) {
        end = i;
      }
    }
    if (strcmp(args[i], "<") == 0) {
      if (i + 1 >= argc) {
        printf("Error [Reached \\n while parsing] (input)");
        return;
      }
      fd = open(args[i + 1], O_RDONLY);
      if (fd == -1) {
        printf("Error [Reached \\n while parsing] (input) \n");
        return;
      }
      if (end > i) {
        end = i;
      }
    }
  }
  args[end] = NULL;
  executefunction(args, argc, output, fd);
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
