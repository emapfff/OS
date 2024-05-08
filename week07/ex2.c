#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


#define MAX_SIZE (500 * 1024 * 1024)

#define CHUNK_SIZE (sysconf(_SC_PAGESIZE) * 1024)

int main() {
    int fd = open("text.txt",O_CREAT | O_RDWR | O_TRUNC, 0666);
    ftruncate(fd, MAX_SIZE);
    char *data = mmap(NULL, MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    int read_file = open("/dev/random", O_RDONLY);
    if (read_file == -1) {
        perror("Cannot open random file");
        exit(EXIT_FAILURE);
    }

	long chars_count = 0;
    int capital_count = 0;
    char* buffer = (char *)malloc(CHUNK_SIZE);
    while(chars_count < MAX_SIZE) {

        ssize_t bytes = read(read_file, buffer, CHUNK_SIZE);
        if (bytes == -1) {
            perror("Cannot read /dev/random");
            exit(EXIT_FAILURE);
        }

        long file_size = chars_count;
        for (ssize_t i = 0; i < bytes && file_size < MAX_SIZE; i++) {
            if (isprint((unsigned char)buffer[i])) {
                if (isupper(buffer[i])) {
                    capital_count++;
                    data[file_size] = tolower(buffer[i]);
                } else {
                    data[file_size] = buffer[i];
                }
                chars_count++;
        		file_size++;
        	}   
        }
        	
    }

    munmap(data, MAX_SIZE);
    close(read_file);

    printf("Number of capital letters: %d\n", capital_count);

    return 0;
}
