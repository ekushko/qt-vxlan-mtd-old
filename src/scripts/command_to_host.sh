#!/bin/bash

ip="$1"
user="$2"
password="$3"
command="$4"

sshpass -p $password ssh $user@$ip $command
