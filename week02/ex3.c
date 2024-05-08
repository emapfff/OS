#include <stdio.h>
#include <string.h>
void convert(char x[], int s, int t){
	if (s < 2 || t < 2 || s > 10 || t > 10){
		printf("cannot convert!");
		return;
	}
	long long result = 0;
	long long base = 1;
	for (int i = strlen(x) - 1; i >= 0; i--){
		int xx = x[i] - '0';
		if (xx >= s) {
			printf("cannot convert!");
			return;		
		}
		result += base * xx;
		base *= s;
	}
	char answer [65]; int num = 0;
	int len = 0;
	while (result > 0){
		answer[len] = (result % t) + '0';
		len++;
		result /= t;
	}
	answer[len] = '\0';
	for (int i = len - 1; i >= 0; i--){
		printf("%c", answer[i]);
	}
	printf("\n");
}

int main(){
	int s, t;
	char x[256];
	scanf("%s %d %d", x, &s, &t);
	convert(x, s, t);	
}
