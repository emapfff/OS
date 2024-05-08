#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>
#define MEMORY_SIZE (10 * 1024 * 1024)

int main(){
	struct rusage usage;
	for (int i = 0; i < 10; i++){
		char* zeros = (char*)malloc(MEMORY_SIZE);
		memset(zeros, 0, MEMORY_SIZE);
		getrusage(RUSAGE_SELF, &usage);
		printf("Memory usage: shared memory size in MB = %ld\n", usage.ru_maxrss/1024);
		sleep(1); 	
	}
	return 0;
}x	
