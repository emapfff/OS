#include <stdio.h>
#include <limits.h>
#include <float.h>

int main (){
	int intVar = INT_MAX;
	unsigned short unsigned_shortVar = USHRT_MAX;
	signed long int signed_long_intVar = LONG_MAX;
	float floatVar = FLT_MAX;
	double doubleVar = DBL_MAX;
	printf("Integer maximum value is %d %s %ld \n", intVar, "and size is ", sizeof(intVar));
	printf("Unsigned short integer maximum value is %d %s %ld \n", unsigned_shortVar, "and size is ", sizeof(unsigned_shortVar));
	printf("Signed long int maximum value is %ld %s %ld \n", signed_long_intVar , "and size is ", sizeof(signed_long_intVar));
	printf("Float maximum value is %f %s %ld \n", floatVar , "and size is ", sizeof(floatVar));
	printf("Double maximum value is %lf %s %ld \n", doubleVar , "and size is ", sizeof(doubleVar));
	return 0;
}
