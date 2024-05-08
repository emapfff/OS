#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/mman.h>
#define MAX_SIZE_PASSWORD 13

int main(){	
	FILE* pid_file = fopen("/tmp/ex1.pid","w");
	int pid = getpid();
	fprintf(pid_file, "%d\n", pid);	
	fflush(pid_file);
	fclose(pid_file);
	printf("From code %d\n", pid);
	int random_file = open("/dev/random", O_RDONLY);
	char* password = (char*)malloc(sizeof(char) * MAX_SIZE_PASSWORD);
	int count = 5;
	char buffer[2];
	sprintf(password, "pass:");
	while (count < MAX_SIZE_PASSWORD){
		ssize_t bytes = read(random_file, buffer, sizeof(char));
		if (bytes == -1){
			printf("Error in random file!\n");
			exit(EXIT_FAILURE);
		}
		if (isprint(buffer[0])){
			password[count] = buffer[0];
			count++;
		}
	}
	close(random_file);
	printf("%s\n", password);
	while(1){
	}
}
