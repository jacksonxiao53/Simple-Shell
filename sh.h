#include "get_path.h"

int pid;
char *which(char *command, struct pathelement *pathlist);
char **where(char *command, struct pathelement *pathlist);
void cd(char *command);
void list(char *dir);

#define PROMPTMAX 64
#define MAXARGS   16
#define MAXLINE   128
