#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define SIZE_ARG 128
#define SIZE_COM 256 
int main (){
	char command[SIZE_COM];
	while (1){
		fgets(command, sizeof(command), stdin);
		int len = strlen(command);
		
		if (len > 0 && command[len - 1] == '\n'){
			command[len - 1] = '\0';
		}
		if (strcmp(command, "exit") == 0) break;
		pid_t pid = fork();
		if (pid < 0){
			printf("Can't fork, error occured\n");
			exit(EXIT_FAILURE);
		} else if (pid == 0){
			char *argv[SIZE_ARG]; 
			char *ptr = strtok(command, " ");
			int i = 0;
			while(ptr != NULL && i < SIZE_ARG){
			
				argv[i++] = ptr;
				ptr = strtok(NULL, " ");		
			}
			argv[i] = NULL;
			if (execvp(argv[0], argv) == -1){
				perror("Could not execute execvp\n");
			}
			exit(0);
		} else {
			int status;
			while(wait(&status)>0);
		} 
	}	
	return 0;
	
}
