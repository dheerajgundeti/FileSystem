#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include <string.h>

#include "process.h"


int main(){
	while (1){
		printf(">");
		char *command;
		command = (char *)calloc(20, sizeof(char));
		gets(command);
		if (strcmp(command, "exit") == 0) break;
		execute1(command);
	}
	return 0;
}



