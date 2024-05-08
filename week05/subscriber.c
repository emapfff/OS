#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define MAX_MESSAGE_SIZE 1024

int main(int argc, char *argv[]) {

    int subscriber_id = atoi(argv[1]);
    char pipe_name[50];

    sprintf(pipe_name, "/tmp/ex1/s%d", subscriber_id);
    int fd = open(pipe_name, O_RDONLY);
    if (fd == -1) {
        perror("Error opening pipe");
        exit(EXIT_FAILURE);
    }

    char message[MAX_MESSAGE_SIZE];
    ssize_t bytes_read;
	while (1){
		while ((bytes_read = read(fd, message, sizeof(message))) > 0) {
		    printf("Message from subscriber: %s\n", message);
		}
    }

    close(fd);

    return 0;
}
