//Jackson Xiao
#include "get_path.h"

char **where(char *command, struct pathelement *p)
{
	char cmd[64];
	char **ch = malloc(100 * sizeof(char*));
	int found = 0;
	while(p){ //check if path is correct then add to array
		sprintf(cmd,"%s/%s",p->element, command);
		if(access(cmd, X_OK) == 0) {
			ch[found] = malloc(strlen(cmd)+1*sizeof(char));
			strcpy(ch[found],cmd);
			found++;
			memset(&cmd[0],0,sizeof(cmd));
			
			
		}
		p = p->next;
	}
	if(found == 0){//none path found
		free(ch);
		return (char **) NULL;
	}
	else{
		return ch;
	}

}

