#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>

pid_t pid;

void handle_ctrl_c(int sig){
	kill(pid, SIGTERM);
	exit(0);
}

int main(){
	FILE* file = fopen("/var/run/agent.pid", "rt");
	char buffer[128];
	if (file == NULL){
		printf("Error: No agent found.\n");
		exit(EXIT_FAILURE);
	}
	fscanf(file, "%d", &pid);
	fclose(file);
	if (pid == -1){
		printf("Error: No agent found.\n");
		exit(EXIT_FAILURE);
	}
	while(1){
		printf("Choose a command {“read”, “exit”, “stop”, “continue”} to send to the agent\n");
		signal(SIGINT, handle_ctrl_c);
		char command[10];
		scanf("%s", command);
		if (strcmp(command, "read") == 0){
			kill(pid, SIGUSR1);
		} else if (strcmp(command, "exit") == 0){
			kill(pid, SIGUSR2);
			exit(0);
		} else if (strcmp(command, "stop") == 0){
			kill(pid, SIGSTOP);
		} else if (strcmp(command, "continue") == 0){
			kill(pid, SIGCONT);
		} else {
			printf("Invalid command\n");
		}
	}
}
