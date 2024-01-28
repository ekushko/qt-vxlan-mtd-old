#!/bin/bash

list_of_hosts="$1"

if [ ! -f $list_of_hosts ]; then
    echo "Файл не найден: $list_of_hosts"
    exit 0
fi

while IFS= read -r line
do
    split_line=($line)
    ip=${split_line[0]}
    user=${split_line[1]}
    password=${split_line[2]}
    should_run=${split_line[3]}
    #ssh-keyscan $ip >> $HOME/.ssh/known_hosts
    ./command_to_host.sh $ip $user $password "kill -9 \$(ps -aux | grep VMTD | awk '{print \$2}')" & continue
    ./send_to_host.sh $ip $user $password ./VMTD & continue
    if [[ $should_run == "run" ]]; then
        ./command_to_host.sh $ip $user $password "cd VMTD; export DISPLAY=:0; nohup ./VMTD -quickstart </dev/null &>/dev/null &" & continue
    fi
done < $list_of_hosts
