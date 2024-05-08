#include <stdio.h>
#include <string.h>
int count (char s[], char c){
	int k = 0;
	for (int i = 0; i < strlen(s); i++){
		if (c == s[i] || c - 'a' == s[i] - 'A' || c - 'A' == s[i] - 'a'){
			k++;	
		}
	}	
	return k;
}

void countAll (char s[]){
	for (int i = 0; i < strlen(s) - 1; i++){
		if (s[i] >= 65 && s[i] < 91) 
			printf("%c:%d, ", s[i] - 'A' + 'a' , count(s, s[i]));
		else 
			printf("%c:%d, ", s[i], count(s, s[i]));
	}
	printf("%c:%d. \n", s[strlen(s) - 1], count(s, s[strlen(s) - 1]));
}

int main(){
	char string[256];
	scanf("%s", string);
	countAll(string);
}
