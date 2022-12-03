#include "parse.h"
int main() {

  while(1){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("[%s] shell@usr $ ",cwd);
    char buffer[100] ;
    fgets(buffer, 100, stdin);
    buffer[strlen(buffer)-1] =0;
    char argc;
    char** test = parse_args(buffer, &argc);
    
    if(strcmp(test[0],"cd") == 0 && argc == 1){
      chdir(test[1]);
    }
    
  }
  // int id, id2;
  // (id = fork()) && (id2 = fork());

  // if (id != 0 && id2 != 0) {
  //   int status;
  //   wait(&status);
  //   printf("%d finished, waited for %ds\n", id, WEXITSTATUS(status));
  //   printf("parent done\n");
  //   exit(0);
  // }
  // srand(getpid());
  // int x = (rand() % 4) + 2;
  // printf("%d %dsec\n", getpid(), x);
  // sleep(x);
  // printf("%d finished\n", getpid());
  // exit(x);
  return 0;
}
