#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h> 

int main(){
	pid_t child_pid1, child_pid2;
	child_pid1 = fork();
	if (child_pid1 == 0){
		printf("Child1: PID = %d, PPID = %d\n", getpid(), getppid());
		clock_t t_child1 = clock();
		for (int i = 0; i < 100000000; i++){}
		t_child1 = clock() - t_child1;
		printf("Child1 execution time is %f milliseconds.\n", ((float)t_child1)/(CLOCKS_PER_SEC * 1000.0));
		exit(0); 
	} 
	child_pid2 = fork();
	if (child_pid2 == 0){
		printf("Child2: PID = %d, PPID = %d\n", getpid(), getppid());
		clock_t t_child2= clock();
		for (int i = 0; i < 100000000; i++){}
		t_child2 = clock() - t_child2;
		printf("Child2 execution time is %f milliseconds.\n", ((float)t_child2)/(CLOCKS_PER_SEC * 1000.0));
		exit(0); 
	}
	printf("Main PID: %d\n", getpid());
	printf("Child1 PID: %d\n", child_pid1);
	printf("Child2 PID: %d\n", child_pid2);
	wait(NULL);
	wait(NULL);
}
