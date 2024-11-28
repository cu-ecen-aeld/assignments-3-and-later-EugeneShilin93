#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "not 2 arg"
    exit 1
fi

directory=$1
search_string=$2

if [ ! -d "$directory" ]; then
    echo "$directory"
    echo "not dir"
    exit 1
fi



file_count=0
line_count=0

while IFS= read -r file; do
    file_match_count=$(grep -n -e "$search_string" "$file" | wc -l)
    if [ "$file_match_count" -gt 0 ]; then
        ((file_count++))
        line_count=$((line_count + file_match_count))
    fi
done < <(find "$directory" -type f)

echo "The number of files are $file_count and the number of matching lines are $line_count "
