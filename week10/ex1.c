#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "dirent.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "sys/inotify.h"
#include "unistd.h"
#include "signal.h"
#include "limits.h"

char source_path[PATH_MAX];

void find_all_hlinks(const char* source){
	DIR *dir = opendir(source_path);	
	struct stat source_stat;
	stat(source, &source_stat);
    ino_t inodeStat = source_stat.st_ino;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
            char* newFiles = (char*)malloc((strlen(source_path) + strlen(entry->d_name)) * sizeof(char)); 
            char* path = realpath(entry->d_name, newFiles);
            struct stat newFileStat;
            if (stat(newFiles, &newFileStat) == -1){
            	return;
            }
            FILE* file = fopen(newFiles, "r");
            char mess[100];
            fgets(mess,100,file);
            if (newFileStat.st_ino == inodeStat && strcmp(source, newFiles	)!=0) {
                printf("Path: %s i-node: %lu Text: %s \n", path, (unsigned long)newFileStat.st_ino, mess);
            }
        }
    }
    closedir(dir);
}
void create_sym_link(const char* source, const char* link){
	symlink(source, link);
}

void unlink_all(const char* source){
	DIR *dir = opendir(source_path);	
	struct stat source_stat;
	stat(source, &source_stat);
    ino_t inodeStat = source_stat.st_ino;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
            char* newFiles = (char*)malloc((strlen(source_path) + strlen(entry->d_name)) * sizeof(char)); 
            char* path = realpath(entry->d_name, newFiles);
            struct stat newFileStat;
            stat(newFiles, &newFileStat);
            if (newFileStat.st_ino == inodeStat && strcmp(source, newFiles)!=0) {
                if (remove(newFiles) == 0){
                	printf("Delete hard link: %s\n", newFiles);
                } else {
                	printf("Did not found diplicate hard links\n");
                }
            }
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]){
	FILE *myfile1 = fopen("myfile1.txt", "w");
    fprintf(myfile1, "Hello world\n");
    fclose(myfile1);
    link("myfile1.txt", "myfile11.txt");
    link("myfile1.txt", "myfile12.txt");
    strcpy(source_path, argv[1]);
    link("myfile1.txt", source_path);
    link("myfile11.txt", source_path);
    link("myfile12.txt", source_path);
    find_all_hlinks("myfile1.txt");
    rename("myfile1.txt", "/tmp/myfile1.txt");
    FILE *myfile11 = fopen("myfile11.txt", "a");
    fprintf(myfile11, "osososososososososo");
    fclose(myfile11);
    char *fullPath = (char*) malloc((strlen(source_path) + strlen("myfile13.txt")) *sizeof(char));
    sprintf(fullPath, "%s/%s", source_path, "myfile13.txt");
    create_sym_link("/tmp/myfile1.txt", fullPath);
    FILE *tmpfile = fopen("/tmp/myfile1.txt", "a");
    fprintf(tmpfile, "\nososososoosososo");
    char *pp = (char*) malloc((strlen(source_path) + strlen("myfile11.txt")) *sizeof(char));
    sprintf(pp, "%s/%s", source_path, "myfile11.txt");
    printf("\n");
    unlink_all(pp);
    printf("\n");
    find_all_hlinks("myfile11.txt");
    return 0;
}
