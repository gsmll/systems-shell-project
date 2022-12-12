# <center style = "color:deepskyblue;"> Systems-Project2-MyShell<br><sup style = "color:gold">Gabriel Small, MeandMyshelf</sup>

## Table of Contents
* [Functions](https://github.com/stuyvesant-cs/project-the-second-myshell-3-small-meandmyshelf/blob/main/README.md#Functions)
* [Failed Features](https://github.com/stuyvesant-cs/project-the-second-myshell-3-small-meandmyshelf/blob/main/README.md#unsuccesful-attemps)
* [Known Bugs](https://github.com/stuyvesant-cs/project-the-second-myshell-3-small-meandmyshelf/blob/main/README.md#Bugs)
* [Headers](https://github.com/stuyvesant-cs/project-the-second-myshell-3-small-meandmyshelf/blob/main/README.md#Headers)

## Features
> * Parse Text into Console
> * Redirect to and from files for input and output using "<"  for input and ">" for output
> * Pipe (one time per command) using "|" 
> * Multiple consecutive commands using ";"
> * Use all Features within the same command
> * Display home path and user name
> * custom color text
## Unsuccesful Attemps
> * Cannot use multiple pipes in the same command (missed class describing pipes and popen())
## Bugs
> * Occasionally redirects will reach end of parsing (unknown reason why, same command works if tried again with same exact characters)
## Headers
> * char** parse_args(char *line, char* args);
> * int executefunction(char **args, int argc, int output, int input);
> * void gatherargs(char *input);
