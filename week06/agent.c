#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>

int pid;

void sigusr1_handler(int sig){
	if (sig == SIGUSR1){
		FILE *file = fopen("text.txt", "r");
		if (file == NULL){
			perror("Cannot open text.txt file");
			exit(EXIT_FAILURE);
		}
		char buffer[128];
		if (fgets(buffer, 128, file) == NULL){
			printf("Cannot read data from text.txt file");
			exit(EXIT_FAILURE);
		}	
		printf("Content from text.txt: %s", buffer);
		fclose(file);
	}
}

void sigusr2_handler(int sig){
	if (sig == SIGUSR2){
		printf("Procces terminating...\n");
		exit(0);
	}
}


int main(){
	pid = getpid();
    FILE *pid_file = fopen("/var/run/agent.pid", "wt");
    if (pid_file == NULL) {
        perror("Does not open pid file");
        exit(EXIT_FAILURE);
    }
    fprintf(pid_file, "%d\n", pid);
    fclose(pid_file);
    
   	signal(SIGUSR1, sigusr1_handler);
   	signal(SIGUSR2, sigusr2_handler);
   	
   	while (1) {
  		sleep(1);
  	}
	return 0;
    
}
