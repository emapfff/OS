#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_PATH 2048
#define MAX_DATA 1024
#define MAX_NAME 63
struct File {
    int id;
    char name[MAX_NAME + 1];
    unsigned long long size;
    char data[MAX_DATA + 1];
    struct Directory *directory;
};
struct Directory{
    char name[MAX_NAME + 1];
    struct File **files; // the maxiximum size is 255, because unsigned char max value is 255
    struct Directory **sub_dirs; // the maxiximum size is 255, because unsigned char max value is 255
    unsigned char nf; 
    unsigned char nd;
    char path[MAX_PATH + 1];
};

void overwrite_to_file(struct File* file, const char* str){
    memcpy(file->data, str, strlen(str));
    file->size = strlen(str);
}

void append_to_file(struct File* file, const char* str){
    strncat(file->data, str, MAX_DATA - file->size);
    file->size = strlen(file->data);
}
void printp_file(struct File* file){
    char tmp[MAX_PATH + 1];
    struct Directory *dir = file->directory;
    if (strcmp(dir->path, "/")){
        strcpy(tmp, dir->path);
        strcat(tmp, "/");
        strcat(tmp, file->name);
    } else {
        strcpy(tmp,"/");
        strcpy(tmp, file->name);
    }
    printf("%s", tmp);
    printf("\n");
}


void add_file(struct File* file, struct Directory* dir){
    if (dir->nf == 0){
        dir->files = (struct File**)malloc(sizeof(struct File*));
    } else {
        dir->files = (struct File**)realloc(dir->files, (dir->nf + 1) * sizeof(struct File*));
    }
    dir->files[dir->nf++] = file;
    file->directory = dir;
}

void show_file(struct File *file) {
    printf("%s ", file->name);
}
void show_dir(struct Directory *dir)
{
    printf("\nDIRECTORY\n");
    printf(" path: %s\n", dir->path);
    printf(" files:\n");
    printf(" [ ");
    for (int i = 0; i < dir->nf; i++){
        show_file(&*(dir->files[i]));
    }
    printf("]\n");
    printf(" directories:\n");
    printf(" { ");
    for (int i = 0; i < dir->nd; i++){
        show_dir(dir->sub_dirs[i]);
    }
    printf("}\n");
}

void add_dir(struct Directory *dir1, struct Directory *dir2){
    if (dir1 && dir2) {
        if (dir1->sub_dirs == NULL) dir1->sub_dirs = (struct Directory **)malloc(sizeof(struct Directory *));
        else dir1->sub_dirs = (struct Directory **)realloc(dir1->sub_dirs, (dir1->nd + 1) * sizeof(struct Directory *));
        dir1->sub_dirs[dir1->nd++] = dir2;
        char temp_path[MAX_PATH + 1];
        if (strcmp(dir1->path, "/")) {
            strcpy(temp_path, dir1->path);
            strcat(temp_path, "/");
            strcat(temp_path, dir2->name);
            strcpy(dir2->path, temp_path);
        }
        else {
            strcpy(temp_path, "/");
            strcat(temp_path, dir2->name);
            strcpy(dir2->path, temp_path);
        }
    }
}
int main() {
    struct Directory root = {.name = "/", .files = NULL, .sub_dirs = NULL, .path = "/",.nf = 0, .nd = 0};
    struct Directory home = {.name = "home", .files = NULL, .sub_dirs = NULL, .path = "", .nf = 0, .nd = 0};
    struct Directory bin = {.name = "bin", .files = NULL, .sub_dirs = NULL, .path = "",.nf = 0, .nd = 0};
    struct File bash = {.name = "bash"};
    add_file(&bash, &bin);
    add_dir(&root, &home);
    add_dir(&root, &bin);

    struct File ex3_1 = {.id = 1, .name = "ex3_1.c"};
    overwrite_to_file(&ex3_1, "int printf(const char * format, ...);");
    struct File ex3_2 = {.id = 2, .name = "ex3_2.c"};
    overwrite_to_file(&ex3_1, "//This is a comment in C language");
    append_to_file(&ex3_1, "int main(){printf('Hello World!')}");
    add_file(&ex3_1, &home);
    add_file(&ex3_2, &home);
    overwrite_to_file(&bash, "Bourne Again Shell!!");
    printp_file(&ex3_1);
    printp_file(&ex3_2);
    printp_file(&bash);
    return 0;
}
