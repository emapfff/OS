#include <stdio.h>

int tribonacci(int n){
	int t0 = 0, t1 = 1, t2 = 1;
	int result = 0;
	if (n == 0) return 0;
	if (n == 1 || n == 2) return 1;
	for (int i = 3; i < n + 1; i++){
		result = t0 + t1 + t2;
		t0 = t1;
		t1 = t2;
		t2 = result;
	}
	return result;
}

int main(){
	int n;
	scanf("%d", &n);
	printf("%d \n", tribonacci(n));
}
