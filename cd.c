//Jackson Xiao
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void cd(char *command){
        char *home = getenv("HOME");//get path to HOME variable
	if(strcmp(command,"") == 0){//cd to HOME
		chdir(home);
	}
	else if (strcmp(command,"-") == 0){//cd to previous
		chdir("..");
	}
	else{
		if (chdir(command) < 0){
			printf("%s: No such file or directory.\n",command);
		}
		chdir(command);//cd to given path
	}
}
