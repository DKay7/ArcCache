#!/usr/bin/env bash
python3  gen_test.py
python3  gen_ans.py ../build/ARC/arc_cache
python3  gen_ans.py ../build/PerfectCache/perfect_cache
python3  cmp_ans.py arc_cache perfect_cache
