#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_SIZE_MES 1024
#define SIZE_PATH_NAME 50
int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    char path_name[SIZE_PATH_NAME];
    mkdir("/tmp/ex1", 0777);
 	int fd;
	while (1){
		char message[MAX_SIZE_MES];
		printf("Enter your message for subscribers: ");
		fgets(message, MAX_SIZE_MES, stdin);		
		for (int i = 0; i < n; i++){
			if (fork() == 0){
				sprintf(path_name,"/tmp/ex1/s%d", i + 1);
    			mkfifo(path_name, 0666);
    			fd = open(path_name, O_RDWR | O_NONBLOCK);
				write(fd, message, strlen(message) + 1);
				close(fd);
				exit(0);
			}
				
		}
		for (int i = 0; i < n; i++) wait(NULL);
	}
	return 0;
}
