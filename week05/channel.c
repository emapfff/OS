#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#define MAX_SIZE_MES 1025

int main(){
	int fd[2];
	if (pipe(fd) == -1){
		printf("\nCould not create a pipe!\n");
		exit(-1);
	}
	pid_t publisher = fork();
	if (publisher == 0){
		close(fd[0]);
		char message[MAX_SIZE_MES];
		printf("Enter your string: ");
		fgets(message, MAX_SIZE_MES, stdin);
		message[strlen(message)] = '\0';
		if( write(fd[1], message, strlen(message) + 1) == -1){
			printf("ERROR WITH WRITE IN PIPE!");
			return 2;
		}
		close(fd[1]);
		exit(0);
	} else {
		wait(NULL);
		close(fd[1]);
		char recieve[MAX_SIZE_MES];
		if (read(fd[0], recieve, MAX_SIZE_MES) == -1){
			printf("ERROR WITH READ FROM PIPE!");
			return 3;
		}
		printf("%s\n", recieve);
		close(fd[0]);
	}
	return 0;
}
