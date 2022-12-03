
#include "parse.h"
char **parse_args(char *line, char* argc) {
  char *t;
  int count = 0;
  int c = 0;
  while (*(line + c) != '\0') {
    if (*(line + c) != ' ' && (*(line + c - 1) == ' '))
      count++;
    c++;
  }
  char **args = malloc(8 * (count + 1));
  c = 0;
  char *cur = line;
  while (c != count + 1) {
    t = strsep(&cur, " ");
    args[c] = t;
    c++;
  }
  *argc = count;
  return args;
}
