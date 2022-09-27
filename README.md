# ARC-cache algorithm realization

Given project implements ARC cache algorithm

---

## Requirements
- To build and run this program you should have installed:
    - C++ compiler with cpp-20 standard support
    - CMake >= 3.10
    - python 3.x to generate and / or run tests 

---

## Run
To run program execute following lines:

1. `mkdir build && cd build`
2. `cmake ..`
3. `make`
4. `cd ARC`
5. `./arc_cache` to run program

---

## Run tests
Test pipeline includes following steps:
1. Generate test files
2. Compute cache hits for each cache algorithms
3. Compare cache hits of different cache algorithms

First, you should do `cd TESTS`.

You may need `--help` option, it's available for each of next steps.

- To generate test files execute `python3 test_generator.py`.
- To compute cache hits for each cache algorithms execute `python3 ans_generator.py PATH_TO_EXECUTABLE`
- To compare cache hits of different cache algorithms `python3 compare_ans.py FIRST_EXEC_NAME SECOND_EXEC_NAME`

For example:
- `python3 test_generator.py`
- `python3 ans_generator.py ../build/ARC/arc_cache`
- `python3 ans_generator.py ../build/LRU/lru_cache`
- `python3 compare_ans.py lru_cache arc_cache`

---

## Test format
Program expects and generates tests in next format:

`CACHE_SIZE` `NUMBER_OF_DATA` 

`DATA`
