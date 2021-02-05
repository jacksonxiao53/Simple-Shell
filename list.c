//Jackson Xiao
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
void list(char *dir){
	struct dirent *entry;
	DIR *openDir;
	if(strcmp(dir,"")==0){ //list current directory
		openDir = opendir(".");
		while((entry = readdir(openDir))){
			printf("%s\n",entry->d_name);
		}
		closedir(openDir);
	}
	else {//list given directory
		printf("\n");
		printf("%s:\n",dir);
		openDir = opendir(dir);
		if(openDir != NULL){
			
			while((entry = readdir(openDir))){
				printf("%s\n",entry->d_name);
			}
			closedir(openDir);
		}
	}
}
