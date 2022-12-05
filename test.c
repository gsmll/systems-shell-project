#include <stdio.h>

int main() {
  FILE *p;
  int ch;

  p = popen("uname", "r"); /* DOS */
                           /*  p = popen("uname","r"); /* Unix */
  if (p == NULL) {
    puts("Unable to open process");
    return (1);
  }
  while ((ch = fgetc(p)) != EOF)
    putchar(ch);
  pclose(p);

  return (0);
}
