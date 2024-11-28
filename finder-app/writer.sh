#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "not 2 arg"
    exit 1
fi

file_path="$1"
string="$2"

directory=$(dirname "$file_path")

if [ ! -d "$directory" ]; then
    echo "create dir"
    mkdir -p "$directory"
fi

echo "$string" > "$file_path"
