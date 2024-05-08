#!/bin/bash 
gcc ex3.c -o ex3
./ex3 3 &
for number in 1 2 3
do 
pstree -p $!
sleep 5
done
./ex3 5 &
for number in 1 2 3 4 5
do 
pstree -p $!
sleep 5
done
rm ex3
