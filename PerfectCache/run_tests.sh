#!/usr/bin/env bash
for dir in Tests/*; do
    if [ -d "$dir" ]; then
        full_path="$dir/test"
        ans=$(cat $full_path | build/PerfectCache/perfect_cache)
        echo "Test $full_path: total hits: $ans"
    fi
done
