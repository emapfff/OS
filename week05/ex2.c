#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAX_SIZE_MES 256

struct Thread {
	pthread_t id;
	int i; 
	char message[MAX_SIZE_MES];
};

void *foo(void *thread){
	struct Thread *thr = (struct Thread *)thread;
	sprintf(thr->message, "Hello from thread %d", thr->i);
	printf("%s ", thr->message);
	printf("%ld\n", thr->id);
	return NULL;
}

int main(){
	int n;
	printf("Enter threads count: ");
	scanf("%d", &n);
	struct Thread threads[n];
	for (int i = 0; i < n; i++){
		threads[i].i = i;
		pthread_create(&threads[i].id, NULL, foo, &threads[i]);
		printf("Thread %d is created ", threads[i].i);
		pthread_join(threads[i].id, NULL);
	}
	return 0;
	
}
