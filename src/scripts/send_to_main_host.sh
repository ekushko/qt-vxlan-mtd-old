#!/bin/bash

ip="$1"
user="$2"
password="$3"

./send_to_host.sh $ip $user $password
sshpass -p $password scp spread_across_infrastructure.sh $user@$ip:/home/$user
sshpass -p $password ssh $user@$ip './spread_across_infrastructure.sh host_list.txt'
