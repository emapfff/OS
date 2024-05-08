#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <fcntl.h>
#include <signal.h>

struct PageTableEntry {
    bool isValid;
    int frameNumber;
    bool isDirty;
    int numReferences;
};

struct PageTableEntry* pageTable;

void printPageTable(struct PageTableEntry* pageTable, int numPages) {
    printf("Page table\n");
    for (int i = 0; i < numPages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
               i, pageTable[i].isValid, pageTable[i].frameNumber, pageTable[i].isDirty, pageTable[i].numReferences);
    }
}

int main(int argc, char* argv[]) {
    int numPages = atoi(argv[1]);
    int pagerPid = atoi(argv[argc - 1]);
    int fd = open("/tmp/ex2/pagetable", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        perror("Unable to create/open the file");
        exit(1);
    }

    size_t pageTableSize = numPages * sizeof(struct PageTableEntry);

    if (ftruncate(fd, pageTableSize) == -1) {
        perror("ftruncate");
        close(fd);
        exit(1);
    }

    pageTable = mmap(NULL, pageTableSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("Initialized page table\n");
    printPageTable(pageTable, numPages);

    for (int i = 2; i <= argc -2; i++) {
        char* request = argv[i];
        char accessType = request[0];
        int pageNumber = atoi(&request[1]);
        printf("%c %c\n", request[0], request[1]);

        switch (accessType) {
            case 'R':
                if (pageTable[pageNumber].isValid == 0) {
                    printf("Read request for page %d\n", pageNumber);
                    printf("It is not a valid page --> page fault\n");
                    printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
                    pageTable[pageNumber].numReferences = getpid();
                    kill(pagerPid, SIGUSR1);
                    kill(getpid(), SIGSTOP);
                    printf("MMU resumed by SIGCONT signal from pager\n");
                    printPageTable(pageTable, numPages);
                    printf("---------------------\n");
                } else if (pageTable[pageNumber].isValid == 1) {
                    printf("Read request for page %d\n", pageNumber);
                    printf("It is a valid page\n");
                    printPageTable(pageTable, numPages);
                    printf("---------------------\n");
                }
                break;

            case 'W':
                printf("Write request for page %d\n", pageNumber);
                if (pageTable[pageNumber].isValid == 0) {
                    printf("It is not a valid page --> page fault\n");
                    printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
                    pageTable[pageNumber].numReferences = getpid();
                    kill(pagerPid, SIGUSR1);
                    kill(getpid(), SIGSTOP);
                    printf("MMU resumed by SIGCONT signal from pager\n");
                    printPageTable(pageTable, numPages);
                    printf("---------------------\n");
                } else {
                    printf("It is a valid page\n");
                    printf("It is a write request, then set the dirty field\n");
                    pageTable[pageNumber].isDirty = 1;
                    printPageTable(pageTable, numPages);
                    printf("---------------------\n");
                }
                break;

            default:
                printf("Unknown access: %s\n", request);
                break;
        }
    }

    close(fd);
    munmap(pageTable, pageTableSize);
    kill(pagerPid, SIGUSR1);

    return 0;
}

