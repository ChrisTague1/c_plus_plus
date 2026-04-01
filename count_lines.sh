#!/bin/bash

ROOT="$(cd "$(dirname "$0")" && pwd)"

total=0

while IFS= read -r file; do
    lines=$(wc -l < "$file")
    printf "%5d  %s\n" "$lines" "${file#$ROOT/}"
    total=$((total + lines))
done < <(find "$ROOT" -name "*.cpp" -o -name "*.h" -o -name "*.hpp" | sort)

echo "-----"
printf "%5d  total\n" "$total"
