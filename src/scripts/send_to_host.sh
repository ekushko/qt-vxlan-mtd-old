#!/bin/bash

ip="$1"
user="$2"
password="$3"

sshpass -p $password scp -r ../../bin/VMTD $user@$ip:/home/$user
