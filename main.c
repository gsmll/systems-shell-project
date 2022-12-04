#include "parse.h"

// executefunction:
// returns the function return value or errno on error or 0 on success
// params:  args [ pointer to args ] 
// executes function in given args

int executefunction(char** args, int argc){

  // tests for exit or cd
   if(strcmp(args[0],"exit") == 0 )
       exit(argc == 2 ? atoi(args[1]) : 0);
   if(strcmp(args[0],"cd") == 0 ){
      if(argc == 2){
      chdir(args[1]);
      return 0;
      }
      else{
      printf("too many args [cd]\n");
      return -1;
      }
    }

  // runs seperate programs
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
// runs execute for each function seperated by a semicolon, takes in stdin for now
void executeargs(char* input){
    char* sect;
    char* search;
    search = strdup(input);
    while((sect = strsep(&search,";")) != NULL){
      char argc;
      char** args = parse_args(sect, &argc);
      executefunction(args, argc);
    }
    return;
}
int main() {
  while(1){
    char cwd [1024];
    getcwd(cwd, sizeof(cwd));
    printf("[%s] shell@%s $ ",cwd,getpwuid(geteuid())->pw_name);
    char buffer[1024] ;
    fgets(buffer, 1024, stdin);
    buffer[strlen(buffer)-1] =0;
    executeargs(buffer);    
  }
  return 0;
}
