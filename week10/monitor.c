#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>

#define LEN_NAME 16
#define MAX_EVENTS 1024
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (MAX_EVENTS * (EVENT_SIZE + LEN_NAME))

int fd, wd;
char path[PATH_MAX];

void traverse_path(const char *path) {
    char entry_path[PATH_MAX];
	struct stat info;
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (!dir) return;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            sprintf(entry_path, "%s/%s", path, entry->d_name);
            if (stat(entry_path, &info) == -1) {
                perror("stat");
                continue;
            } 
            if (S_ISDIR(info.st_mode)) {
                traverse_path(entry_path);
            }
            printf("File: %s, i-node: %lu, links: %lu \n", entry->d_name, info.st_ino, info.st_nlink);
        }
    }
    closedir(dir);
}

void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("\n*****************\n");
        traverse_path(path);
        (void)inotify_rm_watch(fd, wd);
        (void)close(fd);
        exit(0);
    }
}

void print_stat(const char *source) {
	struct stat info;
    if (stat(path, &info) == -1) {
        perror("stat");
        return;
    }
    printf("*****************\n");
    printf("Name: %s, i-node: %lu, links: %lu \n", source, info.st_ino, info.st_nlink);
    printf("*****************\n");
    
}

int main(int argc, char *argv[]) {
    int len;
	struct stat info;
    strcpy(path, argv[1]);
    traverse_path(path);
    printf("*****************\n");
    char buffer[BUF_LEN];
    fd = inotify_init();
    wd = inotify_add_watch(fd, argv[1], IN_ACCESS | IN_CREATE | IN_DELETE | IN_MODIFY | IN_OPEN | IN_ATTRIB);
    int i;
    signal(SIGINT, handle_signal);
    while(1){
        len = read(fd, buffer, BUF_LEN);
        if (len < 0) {
            perror("read");
        }
        for (char *ptr = buffer; ptr < buffer+len;){
		    struct inotify_event *event = (struct inotify_event *)ptr;
		    if (event->len) {
		        char absolute_path[PATH_MAX];
		        sprintf(absolute_path, "%s/%s", argv[1], event->name);
		        if (strcmp(event->name, ".") == 0){
		        	ptr+= sizeof(struct inotify_event) + event->len;
		        }
		        if (event->mask & IN_ACCESS) {
		            printf("%s accessed\n", event->name);
		        } else if (event->mask & IN_CREATE) {
		            printf("%s created\n", event->name);
		            print_stat(absolute_path);
		        } else if (event->mask & IN_DELETE) {
		            printf("%s deleted\n", event->name);
		        } else if (event->mask & IN_MODIFY) {
		            printf("%s modified\n", event->name);
		            print_stat(absolute_path);
		        } else if (event->mask & IN_OPEN) {
		            printf("%s opened\n", event->name);
		           	print_stat(absolute_path);
		        } else if (event->mask & IN_ATTRIB) {
		            printf("%s metadata changed\n", event->name);
		            print_stat(absolute_path);
		        }
		    }
		    ptr+= sizeof(struct inotify_event) + event->len;
        }
     }
    (void)inotify_rm_watch(fd, wd);
    (void)close(fd);
    return 0;
}
