# Simple-Shell
A simple shell in C with some "built-in" commands.

Built-in Commands to support
  - exit 
  - which
  - where 
  - cd - chdir(2) to the directory given; with no arguments, chdir to the home directory; with a '-' as the only argument, chdirs to directory previously in, the same as what tcsh does. 
  - pwd - print the current working directory. 
  - list - with no arguments, lists the files in the current working directory one per line. With arguments, lists the files in each directory given as an argument, with a blank line then the name of the directory followed by a : before the list of files in that directory. 
  - pid - prints the pid of the shell
  - kill - When given just a pid, sends a SIGTERM to the process with that pid using kill(2). When given a signal number (with a - in front of it), sends that signal to the pid.
  - prompt - When ran with no arguments, prompts for a new prompt prefix string. When given an argument make that the new prompt prefix
  - printenv - When ran with no arguments, prints the whole environment. When ran with one argument, call getenv(3) on it. When called with two or more args, print the same error message to stderr that tcsh does.
  - setenv - When ran with no arguments, prints the whole environment, the same as printenv. When ran with one argument, sets that as an empty environment variable. When ran with two arguments, the second one is the value of the first. When ran with more args, print the same error message to stderr that tcsh does
  
  
