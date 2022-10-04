from argparse import ArgumentParser
import os
from random import randint


def parse_args():
    parser = ArgumentParser(description='Compare answer generator args')
    parser.add_argument('first_exec_name', type=str,
                        help='name of the first executable answers')

    parser.add_argument('second_exec_name', type=str,
                        help='name of the second executable answers')

    return parser.parse_args()

def print_compare_sign (first, second):
    if first > second:
        print("\033[91m > \033[0m", end="")
    elif first < second:
        print("\033[91m < \033[0m", end="")
    else:
        print("\033[91m = \033[0m", end="")

def compare_ans (path_to_test_dir, first_exec_name, second_exec_name):
    first_ans_file_name  = os.path.join(path_to_test_dir, first_exec_name + '.ans')
    second_ans_file_name = os.path.join(path_to_test_dir, second_exec_name + '.ans')
    test_file_name = os.path.join(path_to_test_dir, "test")

    if not os.path.isfile(second_ans_file_name) or not os.path.isfile(first_ans_file_name):
        print(f"\t-Can not compare ans because one of following files does not exist:\n"
              f"\t\t'{first_ans_file_name}' '{second_ans_file_name}'\n"
               "\t-Run ans generator first")
        return (0, 0)

    total_data_size = 0
    first_hits  = None
    second_hits = None
    first_exec_time  = None
    second_exec_time = None
    with open(test_file_name, "r") as test_file:
        total_data_size = int(test_file.readline().split()[1])

    with open(first_ans_file_name, "r") as ans_file:
        file_lines = ans_file.readlines()
        first_hits = int(file_lines[0].strip())
        first_exec_time = float(file_lines[1].strip())

    with open(second_ans_file_name, "r") as ans_file:
        file_lines = ans_file.readlines()
        second_hits = int(file_lines[0].strip())
        second_exec_time = float(file_lines[1].strip())

    if first_hits == None or second_hits == None or second_exec_time == None or first_exec_time == None:
        print(f"\t-Can not compare ans because one of\n"
              f"\t\t'{first_ans_file_name}', '{second_ans_file_name}' has broken data\n"
               "\t-Run ans generator first")
        return
    
    first_percents  = (first_hits  / total_data_size) * 100
    second_percents = (second_hits / total_data_size) * 100

    print(f"\033[92m hits:\t{first_exec_name}\033[1m[{first_hits}, {first_percents:.4f}%]\033[0m\033[92m", end="")
    print_compare_sign (first_hits, second_hits)
    print(f"\033[92m {second_exec_name}\033[1m[{second_hits}, {second_percents:.4f}%\033[92m]\033[0m")

    print(f"\033[1;94m time:\t{first_exec_name}\033[1m[{first_exec_time:.5f} sec.]\033[0m\033[92m", end="")
    print_compare_sign (first_exec_time, second_exec_time)
    print(f"\033[1;94m {second_exec_name}\033[1m[{second_exec_time:.5f} sec.\033[1;94m]\033[0m")

    return {
        "total_data_size":  total_data_size, 
        "first_hits":       first_hits, 
        "second_hits":      second_hits,
        "first_time":       first_exec_time,
        "second_time":      second_exec_time
    }

    
def main():
    args = parse_args()
    
    results = {
        "total_data_size":  0, 
        "first_hits":       0, 
        "second_hits":      0,
        "first_time":       0,
        "second_time":      0
    }

    for dir_name in os.listdir():        
        if os.path.isdir(dir_name) and dir_name.startswith("test_"):
            print(f"comparing answer for {dir_name}:")
            first_exec_name = args.first_exec_name
            second_exec_name = args.second_exec_name
            data = compare_ans(dir_name, first_exec_name, second_exec_name)

            results['first_hits']   += data["first_hits"]
            results['second_hits']  += data["second_hits"]
            results['first_time']   += data["first_time"]
            results['second_time']  += data["second_time"]
            results['total_data_size'] += data["total_data_size"]

    print ("\033[91mTotal result:\033[0m")
    
    print(f"\033[92m hits:\t{first_exec_name}\033[1m[{results['first_hits']}]\033[0m\033[92m", end="")
    print_compare_sign (results['first_hits'], results['second_hits'])
    print(f"\033[92m{second_exec_name}\033[1m[{results['second_hits']}\033[92m]\033[0m")

    first_percents  = (results['first_hits']  / results['total_data_size']) * 100
    second_percents = (results['second_hits'] / results['total_data_size']) * 100

    print(f"\033[93m hit percents:\t{first_exec_name}\033[1m[{first_percents:.5f}%]\033[0m\033[93m", end="")
    print_compare_sign (first_percents, second_percents)
    print(f"\033[93m{second_exec_name}\033[1m[{second_percents:.5f}%\033[93m]\033[0m")

    print(f"\033[1;94m time:\t{first_exec_name}\033[1m[{results['first_time']:.5f} sec.]\033[0m\033[92m", end="")
    print_compare_sign (results['first_time'], results['second_time'])
    print(f"\033[1;94m{second_exec_name}\033[1m[{results['second_time']:.5f} sec.\033[1;94m]\033[0m")

    return

if __name__ == "__main__":
    main()
