#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#define SIZE_RAM 8

int access_num = 0;
int fd;
size_t page_table_size;
char (*RAM)[SIZE_RAM + 1];
char (*disk)[SIZE_RAM + 1];
char* algorithm;

int timer = 0;

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    int counter;
};

struct PTE* page_table;
int pages_num;
int frames_num;

// Function to print the page table
void printPTE(struct PTE* page_table, int pages_num) {
    for (int i = 0; i < pages_num; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
}

// Function to print the RAM
void printRAM(char RAM[][SIZE_RAM + 1], int rows) {
    for (int i = 0; i < rows; i++) {
        printf("Frame %d ---> %s\n", i, RAM[i]);
    }
}

// Function to print the disk
void printDISK(char disk[][SIZE_RAM + 1], int rows) {
    printf("Disk array\n");
    for (int i = 0; i < rows; i++) {
        printf("Page %d ---> %s\n", i, disk[i]);
    }
}


int randomm(struct PTE* page_table){
	int r = rand() % pages_num;
	return r;
}

int nfu(struct PTE* page_table){
	int min_count = page_table[0].counter;
	int page_to_evict = INT_MAX;
	for (int i = 1; i < pages_num; i++){
		if (page_table[i].counter < min_count && page_table[i].valid != 0 && page_table[i].frame != -1){
			min_count = page_table[i].counter;
			page_to_evict = i;
		}
	}
	printf("min_count %d\n", min_count);
	return page_to_evict;
}

int aging(struct PTE* page_table){
	int min_count = page_table[0].counter;
	int page_to_evict = INT_MAX;
	if (timer == 1){
		for (int i = 0; i < pages_num;i++){
			page_table[i].counter >>= 1;
			if (page_table[i].counter & 1){
				page_table[i].counter &= ~1;
			}
		}
		timer = 0;
	}
	for (int i = 1; i < pages_num; i++){
		if (page_table[i].counter < min_count && page_table[i].valid != 0 && page_table[i].frame != -1){
			min_count = page_table[i].counter;
			page_to_evict = i;
		}
	}
	return page_to_evict;
	
}
int randomPageReplacement(){
	if (strcmp(algorithm, "random") == 0){
		return randomm(page_table);
	} else if (strcmp(algorithm, "nfu") == 0){
		return nfu(page_table);
	} else if (strcmp(algorithm, "aging") == 0){
		return aging(page_table);
	}
	return -1;
}

// Signal handler function
void signalHandler(int signum) {
    bool finish = false;
    int index = 0;
    if (signum == SIGUSR1) {
        for (int i = 0; i < pages_num; i++) {
            if (page_table[i].referenced != 0) {
                index = i;
                printf("A disk access request from MMU Process (pid = %d)\n", page_table[i].referenced);
                printf("Page %d is referenced\n", i);
                bool flag = false;
                for (int j = 0; j < frames_num; j++) {
                    if ((strlen(RAM[j]) == 0)) {
                        printf("We can allocate it to free frame %d\n", j);
                        printf("Copy data from the disk (page = %d) to RAM (frame = %d)\n", i, j);
                        strcpy(RAM[j], disk[i]);
                        flag = true;
                        page_table[i].valid = 1;
                        page_table[i].frame = j;
                        break;
                    }
                }
                if (flag == false) {
                	printf("soijaosjpofijsaofiaj\n");
                	int page_to_evict = randomPageReplacement();
                    for (int j = 0; j < pages_num; j++) {
                        if (page_to_evict == page_table[j].frame) {
                            if (page_table[j].dirty == true) page_table[j].dirty = false;
                            strcpy(disk[j], RAM[page_to_evict]);
                            page_table[j].frame = -1;
                            page_table[j].valid = 0;
                            break;
                        }
                    }
                    timer++;
                    printf("We do not have free frames in RAM\nChoose a victim page using %s %d\n", algorithm, page_to_evict);
                    printf("Replace/Evict it with page %d to be allocated to frame %d\n", i, page_to_evict);
                    printf("Copy data from the disk (page = %d) to RAM (frame = %d)\n", i, page_to_evict);
                    strcpy(RAM[page_to_evict], disk[i]);
                    page_table[i].valid = 1;
                    page_table[i].frame = page_to_evict;
                }
                finish = true;
            }
        }
        if (finish == false) {
            remove("/tmp/ex2/pagetable");
            munmap(page_table, page_table_size);
            printf("Access number: %d\n", access_num);
            exit(0);
        }
        printf("RAM array\n");
        printRAM(RAM, frames_num);
        access_num++;
        printf("Disk accesses is %d so far\n", access_num);
        printf("Resume mmu process\n");
        printf("-------------------\n");
        kill(page_table[index].referenced, SIGCONT);
        page_table[index].referenced = 0;
    }
}

int main(int argc, char* argv[]) {
    printf("Pager PID %d\n", getpid());
    pages_num = atoi(argv[1]);
    frames_num = atoi(argv[2]);
    algorithm = argv[3];
    srand(time(NULL));
    int random_file = open("/dev/urandom", O_RDONLY);
    if (random_file == -1) {
        printf("ERROR\n");
        exit(EXIT_FAILURE);
    }
    unsigned char random_char;
    RAM = malloc(frames_num * sizeof(*RAM));
    disk = malloc(pages_num * sizeof(*disk));
    memset(RAM, 0, sizeof(RAM));
    int count = 0;
    char buffer[2];
    for (int i = 0; i < pages_num; i++) {
        while (count < SIZE_RAM) {
            ssize_t bytes = read(random_file, buffer, sizeof(char));
            if (isprint(buffer[0])) {
                disk[i][count] = buffer[0];
                count++;
            }
        }
        disk[i][SIZE_RAM] = '\0';
        count = 0;
    }
    close(random_file);
    mkdir("/tmp/ex2", 0666);
    fd = open("/tmp/ex2/pagetable", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
    page_table_size = pages_num * sizeof(struct PTE);
    if (ftruncate(fd, page_table_size) == -1) {
        perror("ftruncate");
        close(fd);
        exit(1);
    }

    page_table = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    for (int i = 0; i < pages_num; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }
    printf("Initialized page table\n");
    printPTE(page_table, pages_num);
    printf("-----------------------\n");
    printf("Initialized RAM\n");
    printRAM(RAM, frames_num);
    printf("-----------------------\n");
    printf("Initialized disk\n");
    printDISK(disk, pages_num);

    signal(SIGUSR1, signalHandler);
    while (1) {

    }

    close(fd);

    return 0;
}

