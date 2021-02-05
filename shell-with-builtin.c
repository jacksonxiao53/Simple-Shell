//Jackson Xiao
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <glob.h>
#include "sh.h"

extern char **environ;
int main(int argc, char **argv, char **envp)
{
	char	buf[MAXLINE];
	char    *arg[MAXARGS];  // an array of tokens
	char    *ptr;
        char    *pch;
	pid_t	pid;
	int	status, i, arg_no;
	char *ctlD;

	char prefix[MAXLINE];
	prefix[0] = '\0';
	char *cwd = getcwd(NULL,0);
	fprintf(stdout,"%s [%s]> ",prefix,cwd);
	fflush(stdout);
	signal(SIGINT,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	signal(SIGTERM,SIG_IGN);
	while ((ctlD = fgets(buf, MAXLINE, stdin)) != NULL) {
		if (strlen(buf) == 1 && buf[strlen(buf) - 1] == '\n')
		  goto nextprompt;  // "empty" command line

		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0; /* replace newline with null */
		// parse command line into tokens (stored in buf)
		arg_no = 0;
                pch = strtok(buf, " ");
                while (pch != NULL && arg_no < MAXARGS)
                {
		  arg[arg_no] = pch;
		  arg_no++;
                  pch = strtok (NULL, " ");
                }
		arg[arg_no] = (char *) NULL;

		if (arg[0] == NULL)  // "blank" command line
		  goto nextprompt;

		/* print tokens
		for (i = 0; i < arg_no; i++)
		  printf("arg[%d] = %s\n", i, arg[i]);
                */
		if (strcmp(arg[0], "exit") == 0){ // built-in command exit
		  printf("Executing built-in [exit]\n");
		  free(cwd);
		  exit(0);	
		}

                if (strcmp(arg[0], "pwd") == 0) { // built-in command pwd 
		  printf("Executing built-in [pwd]\n");
	          ptr = getcwd(NULL, 0);
                  printf("%s\n", ptr);
                  free(ptr);
	        }
		else
                if (strcmp(arg[0], "which") == 0) { // built-in command which
		  struct pathelement *p, *tmp;
                  char *cmd;
                    
		  printf("Executing built-in [which]\n");

		  if (arg[1] == NULL) {  // "empty" which
		    printf("which: Too few arguments.\n");
		    goto nextprompt;
                  }
		  int i = 1;
		  while(i < arg_no){ // iterate through all parameters

		  	p = get_path();
           /***
		  tmp = p;
		  while (tmp) {      // print list of paths
		    printf("path [%s]\n", tmp->element);
		    tmp = tmp->next;
                  }
           ***/

                  	cmd = which(arg[i], p);
                  	if (cmd) {
		   		printf("%s\n", cmd);
                    		free(cmd);
                  	}
		  	else               // argument not found
		    		printf("%s: Command not found\n", arg[1]);

		  	while (p) {   // free list of path values
		     		tmp = p;
		     		p = p->next;
		     		free(tmp->element);
		     		free(tmp);
                  		}
			i++;
	        	}
		}
		else if(strcmp(arg[0],"where") == 0) {// built-in command where
		    struct pathelement *p, *tmp;
		    char **cmd;

		    printf("Executing built-in [where]\n");

		    if(arg[1] == NULL){ // "empty" where
		      printf("where: Too few arguments. \n");
		      goto nextprompt;
		    }

		    int i = 1;
		    while(i< arg_no){ //iterate through all parameters
			p = get_path();
		    	cmd = where(arg[i],p);
		    	int len = sizeof(cmd)/sizeof(cmd[0]);
		    	if(cmd != NULL){
			    for(int i = 0; i<len;i++){
				    printf("%s\n",cmd[i]);
				    free(cmd[i]);
			    }
		    	}
		    	free(cmd);
		    	while (p) {
		       		tmp = p;
		       		p = p->next;
	   	       		free(tmp->element);
   		       		free(tmp);
		    		}
			i++;

			}	
		}	    
		else if(strcmp(arg[0],"cd")== 0 ){ // built-in command cd
			printf("Executing built-in [cd]\n");
			if(arg_no < 3){
				if(arg[1] == NULL){ //no parameters
					cd("");
				}
				else{
					cd(arg[1]);// one parameter
				}
			}
			else{
				printf("cd: Too many arguments.\n");
			}
		}
		else if(strcmp(arg[0],"list") == 0){//built-in command list
			printf("Executing built-in [list]\n");
			if(arg[1] == NULL){// no parameter
				list("");
			}
			else{
				for(int i = 1; i<arg_no;i++){
					list(arg[i]);
				}
			}
		}
		else if (strcmp(arg[0],"pid") == 0){//built-in command pid
			printf("Executing built-in [pid]\n");
			pid_t shellpid = getpid();
			printf("%d\n",shellpid);
		}
		else if (strcmp(arg[0],"kill")==0){//built in command kill
			int r = 0;
			printf("Executing built-in [kill]\n");
			if(arg[1] == NULL){ //no parameters
				printf("kill: Too few arguments\n");
			}
			else if(arg_no == 2){// no signal number
				r = kill(atoi(arg[1]),15);
			}
			else if(arg_no == 3){ //with signal number
				char *signal = arg[1]+1;
				int signalNum= atoi(signal);
				printf("signal # [%d]\n",signalNum);
				free(cwd);
				r = kill(atoi(arg[2]),signalNum);
			}
			else {
				printf("kill syntax error\n");
			}

			if(errno == ESRCH){
				if(arg_no == 2){
					printf("%s: No such process\n",arg[1]);
				}
				else if(arg_no == 3){
					printf("%s: No such process\n",arg[2]);
				}
			}
		}
		else if(strcmp(arg[0],"prompt") == 0){//built-in command prompt
			printf("Executing built-in [prompt]\n");
			if(arg[1] == NULL){//no parameter
				printf("input prompt prefix: ");
				if(fgets(prefix,MAXLINE,stdin) != NULL){
					if(prefix[strlen(prefix)-1]=='\n'){
						prefix[strlen(prefix)-1]=0;
					}
				}
			}
			else{//with parameter
				strcpy(prefix,arg[1]);
			}
		}
		else if (strcmp(arg[0],"printenv")==0){// built-in command printenv
			printf("Executing built-in [printenv]\n");
			if(arg[1]== NULL){//no parameter
				int i = 0;
				while(environ[i] != NULL){
					printf("%s\n",environ[i]);
					i++;
				}
			}
			else if(arg[1] != NULL && arg[2] == NULL){
				if(getenv(arg[1])!= NULL){//one parameter
					printf("%s\n",getenv(arg[1]));
				}
			}
			else{
				printf("printenv: Too many arguments.\n");
			}
		}
		else if(strcmp(arg[0],"setenv") == 0){//built-in command setenv
			printf("Executing built-in [setenv]\n");
			if(arg[1] == NULL){//no parameter
				int i = 0;
				while(environ[i] != NULL){
					printf("%s\n",environ[i]);
					i++;
				}
			}
			else if(arg[1] != NULL && arg[2] == NULL){//one parameter
				setenv(arg[1],"",1);
			}
			else if (arg[2] != NULL && arg[3] == NULL){//two parameters
				setenv(arg[1],arg[2],1);
			}
			else{
				printf("setenv: Too many arguements.\n");
			}
		}	
		else {  // external command
		  if ((pid = fork()) < 0) {
			printf("fork error");
		  } else if (pid == 0) {		/* child */
			                // an array of aguments for execve()
	                char    *execargs[MAXARGS]; 
		        glob_t  paths;
                        int     csource, j;
			char    **p;
			struct pathelement *path, *tmp;
                        j = 0;
		
			char c = arg[0][0];
			if(c != '/' && c != '.'){//find the path of command
				path = get_path();
				char *cmdPath;
				cmdPath = which(arg[0],path);
				if(cmdPath == NULL){
					printf("%s: Command not found.\n",arg[0]);
				}
				execargs[j] = malloc(strlen(cmdPath)+1);
				strcpy(execargs[0],cmdPath);
				while (path) {   // free list of path values
                    			 tmp = path;
                    			 path = path->next;
                    			 free(tmp->element);
                    			 free(tmp);
                 		 }
				free(cmdPath);
			}
			else{
				execargs[j] = malloc(strlen(arg[0])+1);
                        	strcpy(execargs[0], arg[0]);  // copy command

			}
		        j = 1;
		        for (i = 1; i < arg_no; i++) // check arguments
			  if (strchr(arg[i], '*') != NULL) { // wildcard!
			    csource = glob(arg[i], 0, NULL, &paths);
                            if (csource == 0) {
                              for (p = paths.gl_pathv; *p != NULL; ++p) {
                                execargs[j] = malloc(strlen(*p)+1);
				strcpy(execargs[j], *p);
				j++;
                              }
                           
                              globfree(&paths);
                            }
                          }
			  else{
				  execargs[j] = malloc(strlen(arg[i])+1);
				  strcpy(execargs[j],arg[i]);
				  j++;
			  }
                        execargs[j] = NULL;

			
			execve(execargs[0],execargs,NULL);
			printf("couldn't execute: %s", buf);
			exit(127);
		  }

		  /* parent */
		  if ((pid = waitpid(pid, &status, 0)) < 0)
			printf("waitpid error");
/**
                  if (WIFEXITED(status)) S&R p. 239 
                    printf("child terminates with (%d)\n", WEXITSTATUS(status));
**/
                }

           nextprompt:
		free(cwd);
		cwd = getcwd(NULL,0);
		fprintf(stdout,"%s [%s]> ",prefix,cwd);
		fflush(stdout);
	}
	if(ctlD == NULL){
		printf("Use \"exit\" to leave tcsh.\n");
		goto nextprompt;
	}
	exit(0);
}
