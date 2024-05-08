#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void* max_int(const void* a, const void* b) {
    if (*(int *)a > *(int *)b) return (void *) a;
    else return (void *) b;
}
void* max_double(const void* a, const void* b) {
    if (*(double *)a > *(double *)b) return (void *) a;
    else return (void *) b;
}

void* add_int(const void* a, const void* b) {
    int *res = (int *)malloc(sizeof(int));
    *res = (*(int *)a + *(int *)b);
    return (void *)res;

}
void* add_double(const void* a, const void* b) {
    double *res = (double *)malloc(sizeof(double));
    *res = (*(double *)a + *(double *)b);
    return (void *)res;
}

void* multiplication_int(const void* a, const void* b){
    int *res = (int *)malloc(sizeof(int));
    *res = (*(int *)a * *(int *)b);
    return (void *)res;
}

void* multiplication_double(const void* a, const void* b){
    double *res = (double *)malloc(sizeof(double));
    *res = (*(double *)a * *(double *)b);
    return (void *)res;
}
void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)){
    void* answer = initial_value;
    for (int i = 0; i < n; i++){
        answer = opr(answer, (base + i * size));
    }
    return answer;
}

int main(){
    int int_arr[5] = {14, 16, 18, 20, 22};
    double double_arr[5] = {14.14, 16.16, 18.18, 20.20, 22.22};
    int n = 5;
    int initial_value_int = 0;
    void* sum_int = aggregate(int_arr, sizeof(int), n, &initial_value_int, add_int);
    printf("%d\n", *(int *)(sum_int));
    double initial_value_double = 0.0;
    void* sum_double = aggregate(double_arr, sizeof(double), n, &initial_value_double, add_double);
    printf("%f\n", *(double *)(sum_double));

    initial_value_int = 1;
    void* mul_int = aggregate(int_arr, sizeof(int), n, &initial_value_int, multiplication_int);
    printf("%d\n", *(int *)(mul_int));
    initial_value_double = 1.0;
    void* mul_double = aggregate(double_arr, sizeof(double), n, &initial_value_double, multiplication_double);
    printf("%f\n", *(double *)(mul_double));

    initial_value_int = INT_MIN;
    void* max_int_val = aggregate(int_arr, sizeof(int), n, &initial_value_int, max_int);
    printf("%d\n", *(int *)(max_int_val));
    initial_value_double = -1.0 * __DBL_MAX__;
    void* max_double_val = aggregate(double_arr, sizeof(double), n, &initial_value_double, max_double);
    printf("%f\n", *(double *)(max_double_val));

    return 0;
}
