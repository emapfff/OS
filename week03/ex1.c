#include<stdio.h>
#include <stdlib.h>

int const_tri(int n, int *const p){
	if (n == 1 || n == 2) return 1;
	if (n == 3) return 2;
	int tmp = 0;
	for (int i = 4; i < n + 1; i++){
		tmp = *p + *(p + 1) + *(p + 2);
		*p = *(p + 1);
		*(p + 1) = *(p + 2);
		*(p + 2) = tmp;
	}
	return tmp;
	
}
int main(){
	int n;
	const int x = 1; 
	const int *q = &x;
	int* const p = (int* const)malloc(3 * sizeof(int));
	*p = *q;
	*(p + 1) = *q;
	*(p + 2) = 2 * (*q);
	printf("Values of p: %d %d %d ", *p, *(p + 1), *(p + 2));
	printf("\nMemory adresses: %p %p %p ", p, (p + 1), (p + 2));
	if (&p[1] == &p[0] + 1 && &p[2] == &p[1] + 1) printf("\nCells are contiguous.\n");
	else printf("\nCells are not contiguous.\n"); 
	free(p);
	return 0;
}			

