#!/bin/bash 
pid=$(<"/tmp/ex1.pid")
echo $pid
heap_line=$(sudo grep "heap" /proc/$pid/maps)
start="0x$(sudo echo "$heap_line" | awk '{print $1}' | cut -d'-' -f1)"
end="0x$(sudo echo "$heap_line" | awk '{printf $1}' | cut -d'-' -f2)"
password=$(sudo xxd -s $start -l $(($end - $start)) /proc/$pid/mem | less | grep -o "pass:[^ ]*" )
echo "Address: " $start
echo "Password: " $password
kill -9 $pid

