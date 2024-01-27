#!/bin/bash

list_of_hosts=$1

if [ ! -f $list_of_hosts ]; then
    echo "Файл не найден: " + $list_of_hosts
    exit 0
fi

while IFS= read -r line
do
  ./send_to_host.sh "$line"
done < $list_of_hosts
