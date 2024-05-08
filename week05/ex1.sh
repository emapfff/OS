#!/bin/bash

read -p "Enter count of subscribers: " n

gcc publisher.c -o publisher
gcc subscriber.c -o subscriber


for ((i = 1; i <= n; i++)) do
	gnome-terminal -- bash -c "./subscriber $i"
done
gnome-terminal -- bash -c "./publisher $n"


rm -r subscriber
rm -r publisher

