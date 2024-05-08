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

#define SIZE_RAM 8

int access_num = 0;
int fd;
size_t page_table_size;
char (*RAM)[SIZE_RAM + 1];
char (*disk)[SIZE_RAM + 1];

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
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
                    int random_value = rand() % frames_num;
                    for (int j = 0; j < pages_num; j++) {
                        if (random_value == page_table[j].frame) {
                            if (page_table[j].dirty == 1) page_table[j].dirty = 0;
                            strcpy(disk[j], RAM[random_value]);
                            page_table[j].frame = -1;
                            page_table[j].valid = 0;
                            break;
                        }
                    }
                    printf("We do not have free frames in RAM\nChoose a random victim page %d\n", random_value);
                    printf("Replace/Evict it with page %d to be allocated to frame %d\n", i, random_value);
                    printf("Copy data from the disk (page = %d) to RAM (frame = %d)\n", i, random_value);
                    strcpy(RAM[random_value], disk[i]);
                    page_table[i].valid = 1;
                    page_table[i].frame = random_value;
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

