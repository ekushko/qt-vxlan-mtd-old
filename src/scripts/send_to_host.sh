#!/bin/bash

ip="$1"
user="$2"
password="$3"
filepath="$4"

if [ -d $filepath ]; then
    sshpass -p $password scp -r $filepath $user@$ip:/home/$user
elif [ -f $filepath ]; then 
    sshpass -p $password scp $filepath $user@$ip:/home/$user
else
    echo "Not found: $filepath"
fi
