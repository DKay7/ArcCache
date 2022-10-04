#!/usr/bin/env bash
for dir in Tests/*; do
    if [ -d "$dir" ]; then
        full_path="$dir/test"
        ans=$(cat $full_path | build/ARC/arc_cache)
        echo "Test $full_path: total hits: $ans"
    fi
done
