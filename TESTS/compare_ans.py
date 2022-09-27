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

    
def compare_ans (path_to_test_dir, first_exec_name, second_exec_name):
    first_ans_file_name  = os.path.join(path_to_test_dir, first_exec_name + '.ans')
    second_ans_file_name = os.path.join(path_to_test_dir, second_exec_name + '.ans')

    if not os.path.isfile(second_ans_file_name) or not os.path.isfile(first_ans_file_name):
        print(f"\t-Can not compare ans because one of following files does not exist:\n"
              f"\t\t'{first_ans_file_name}' '{second_ans_file_name}'\n"
               "\t-Run ans generator first")
        return (0, 0)

    first_hits  = None
    second_hits = None

    with open(first_ans_file_name, "r") as ans_file:
        first_hits = int(ans_file.read().strip())

    with open(second_ans_file_name, "r") as ans_file:
        second_hits = int(ans_file.read().strip())

    if first_hits == None or second_hits == None:
        print(f"\t-Can not compare ans because one of\n"
              f"\t\t'{first_ans_file_name}', '{second_ans_file_name}' has broken data\n"
               "\t-Run ans generator first")
        return

    print(f"\033[92m\t{path_to_test_dir}: {first_exec_name}\033[1m[{first_hits}]\033[0m\033[92m", end="")
    print("\033[91m > \033[92m" if first_hits > second_hits else "\033[94m < \033[92m" if first_hits < second_hits else "\033[93m = \033[92m", end="")
    print(f"{second_exec_name}\033[1m[{second_hits}\033[92m]\033[0m")
    return [first_hits, second_hits]

    
def main():
    args = parse_args()
    
    result_list = [0, 0]
    for dir_name in os.listdir():        
        if os.path.isdir(dir_name) and dir_name.startswith("test_"):
            print(f"comparing answer for {dir_name}:")
            fisrt, second = compare_ans(dir_name, args.first_exec_name, args.second_exec_name)

            result_list[0] += fisrt
            result_list[1] += second
        
    print ("\033[91mTotal result:\033[0m")
    
    print(f"\033[92m\t{args.first_exec_name}\033[1m[{result_list[0]}]\033[0m\033[92m", end="")
    
    print("\033[91m > \033[92m" if result_list[0] > result_list[1] else "\033[94m < \033[92m" if result_list[0] < result_list[1] else "\033[93m = \033[92m", end="")

    print(f"{args.second_exec_name}\033[1m[{result_list[1]}\033[92m]\033[0m")

    return

if __name__ == "__main__":
    main()
