#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h> 
#define SIZE 120

int dot_product(int *u, int *v, int s, int e){
	int result = 0;
	for (int i = s; i < e; i++){
		result += u[i] * v[i];
	} 
	return result;
}

int main(){
	int u[SIZE], v[SIZE];
	int n;
	for (int i = 0; i < SIZE; i++){
		u[i] = rand() % 100;
		v[i] = rand() % 100;
	}
	printf("Enter number of processes: ");
	scanf("%d", &n);
	FILE *file = fopen("temp.txt", "w+");
	FILE *file_out = fopen("temp.txt", "r");
	if (file == NULL){
		printf("\nError, file could not open!\n");
		exit(1);
	}
	int count_components = SIZE / n;
	for (int i = 0; i < n; i++){
		pid_t child = fork();	
		if (child == -1){
			printf("Error!\n");
		} else if (child == 0){
			int start = i * count_components;
			int end = start + count_components;
			int res = dot_product(u, v, start, end);
			fprintf(file, "%d\n", res);
			exit(0);
		}
	}
	for (int i = 0; i < n; i++){
		wait(NULL);
	}
	int answer = 0, i = 0;
	while (!feof(file_out)){
		fscanf(file_out, "%d", &i);
		answer += i;
	}
	answer -= i;
	fclose(file);
	fclose(file_out);
	printf("\nDot product equals to %d\n", answer); 
	return 0;
}
