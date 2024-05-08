#!bin/bash
echo "File types in /dev"
ls -l /dev
echo "File types in /etc"
ls -l /etc
echo "Count of directories in /etc"
ls -l /etc | grep ^d | wc -l
echo "File type of ex1.c before compilation"
file ex1.c
gcc ex1.c -o ex1
echo "File type of ex1.c after compilation"
file ex1.c
./ex1
echo "File type of ex1.c with 'Привет, мир!'"
file ex1.c 
echo "The difference between the file ex1.c with the output 'Hello, world!' and the file ex1.c with the output 'Привет, мир!' is that the first contains ASCII text characters, and the second contains Unicode text. Since the first file contains only English characters, the file is identified as ASCII, and the second contains Russian characters, they are identified as Unicode, where it is an international character encoding standard that provides a unique number to each character in different languages and scripts, which makes almost everything symbols available for different platforms, programs and devices."

