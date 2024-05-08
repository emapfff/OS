#!/bin/bash
gcc -pthread ex4.c -o ex4
rm -r ex4_res.txt
{ time ./ex4 10000000 1; } 2>> ex4_res.txt
{ time ./ex4 10000000 2; } 2>> ex4_res.txt
{ time ./ex4 10000000 4; } 2>> ex4_res.txt
{ time ./ex4 10000000 10; } 2>> ex4_res.txt
{ time ./ex4 10000000 100; } 2>> ex4_res.txt
rm -r ex4

