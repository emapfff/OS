#include <stdio.h>
#include <string.h>

int main(){
	char string [256];
	char c;
	int len = 0;
	while (1){
		c = getchar();
		if (c == '.' || c == '\n')
			break;
		string[len] = c;
		len++; 
	}
	string[len] = '\0';
	for (int i = len - 1; i >= 0; i--){
		printf("%c", string[i]);
	}
	printf("\n");
	return 0;
}
