# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <pthread.h>
bool is_prime ( int n){
	if (n <= 1) return false ;
	int i = 2;
	for (; i * i <= n; i++) {
		if (n % i == 0)
			return false;
	}
	return true;
}
int primes_count ( int a, int b)
{
	int ret = 0;
	for (int i = a; i < b; i++)
		if (is_prime (i) != 0)
			ret ++;
	return ret ;
}
typedef struct prime_request
{
	int a, b;
} prime_request ;
void * prime_counter ( void *arg)
{
	prime_request* req = (prime_request*) arg; 
	int * count = malloc(sizeof(int));
	*count = primes_count(req->a, req->b);
	return (( void *) count);
}

int main(int argc, char* argv[]){
	if (argc < 2){
		printf("Please enter 2 arguments\n");
		exit(0);
	}
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	prime_request pq[m];
	pthread_t threads [m];
	int count = 0;
	int del = n / m;
	for (int i = 0; i < m; i++){
		int a = i * del;
		int b = ((i + 1) * del); 
		if (i == m - 1) b = n;
		pq[i].a = a;
		pq[i].b = b;
		pthread_create(&threads[i], NULL, prime_counter, (void *)&pq[i]);
	}
	for (int i = 0; i < m; i++){
		void* res; 
		pthread_join(threads[i], &res);
		count += *(int *) res;
		free(res);
	}
	printf("Count of threads: %d. Count of primes: %d \n", m, count);
	return 0;
}
