
#include "parse.h"


// parses through arguments, first counting amount of args to malloc arg array, then going enumerating each argument value to the array args.
// takes in parameters argc [char for arg array length] and line [line to be parsed]
// returns array of arguments from line.
char **parse_args(char *line, char* argc) {
  char *t;
  int count = 0;
  int c = 0;
  while (*(line + c) != '\0') {
    if (*(line + c) != ' ' && ((*(line + c - 1) == ' ') || (*(line + c - 1) == 0)))
      count++;
    c++;
  }
  char **args = malloc(8 * (count + 1));
  c = 0;
  char *cur = line;
  while (c != count ) {
    t = strsep(&cur, " ");
    if(t[0] != 0){
    args[c] = t;
    c++;
    }
  }
  args[count] = NULL;
  *argc = count;
  return args;
}
