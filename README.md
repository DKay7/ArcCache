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
2. `cmake -DCMAKE_BUILD_TYPE=Release ..`
3. `make`
4. `cd ARC`
5. `./arc_cache` to run program

or

4. `cd PerfectCache`
5. `./perfect_cache` to run perfect cache
---

## Run tests
Test pipeline includes following steps:
1. Generate test files
2. Compute cache hits for each cache algorithms
3. Compare cache hits of two different cache algorithms

First, you should do `cd TESTS`.

You may need `--help` option, it's available for each of next steps.

- To generate test files execute `python3 gen_test.py`.
- To compute cache hits for each cache algorithms execute `python3 gen_ans.py RELATIVE_PATH_TO_EXECUTABLE`
- To compare cache hits of different cache algorithms `python3 cmp_ans.py FIRST_EXEC_NAME SECOND_EXEC_NAME`
- To view available options run one of the following commands: 
    - `python3 gen_test.py -h`
    - `python3 gen_ans.py -h`
    - `python3 cmp_ans.py -h`


For example:
- `python gen_test.py`
- `python gen_ans.py ../build/ARC/arc_cache`
- `python gen_ans.py ../build/PerfectCache/perfect_cache`
- `python3 cmp_ans.py arc_cache perfect_cache`

To run only ARC cache tests you may want to do next steps:
- Make sure you're in projects' root
- `./ARC/run_tests.sh`

---

## Test format
Program expects and generates tests in next format:

`CACHE_SIZE` `NUMBER_OF_DATA` 

`DATA`

## Results
We have reached next results on given here tests:
```
Total result:
    hits:  arc_cache[1189] < perfect_cache[21333]
    hit percents:  arc_cache[0.29725%] < perfect_cache[5.33325%]
    time:  arc_cache[0.13000 sec.] < perfect_cache[58.22000 sec.]
```
Perfect cache is better than arc by **~17 times** because in first one we "can look up through future". But,

Perfect cache execution time is worse than arc one by **~447 times**(!!), because in perfect cache we have to check up the future in each push. 