#include "parse.h"

// executefunction:
// returns the function return value or errno on error
// params:  args [ pointer to args ] 
// executes function in given args

int executefunction(char** args){
  int id = fork();
  
  if(id != 0){
      
      int status;
      int w = wait(&status);
      if(w){
        if (WIFEXITED(status)) {
          return WEXITSTATUS(status);
        }
        if (WIFSIGNALED(status)) {
          return WTERMSIG(status);
        }
        return -1;
      }
      else{
        return errno;
      }
  }
  execvp(args[0], args);
  return errno;
}

int main() {
  while(1){
    char cwd [1024];
    getcwd(cwd, sizeof(cwd));
    printf("[%s] shell@%s $ ",cwd,getpwuid(geteuid())->pw_name);
    char buffer[100] ;
    fgets(buffer, 100, stdin);
    buffer[strlen(buffer)-1] =0;
    char argc;
    char** args = parse_args(buffer, &argc);
    if(strcmp(args[0],"exit") == 0 )
       exit(argc == 1 ? atoi(args[1]) : 0);
    if(strcmp(args[0],"cd") == 0 ){
      if(argc == 1)
      chdir(args[1]);
      else
      printf("too many args [cd]\n");
    }
    else executefunction(args);
    

    
    
    
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
