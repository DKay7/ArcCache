from argparse import ArgumentParser
import os
from random import randint


def parse_args():
    parser = ArgumentParser(description='Process answer generator args')
    parser.add_argument('exec_path', type=str,
                        help='path to executable file to run tests')

    return parser.parse_args()

    
def generate_ans (path_to_test_dir, exec_path):
    test_file_name = os.path.join(path_to_test_dir, "test")
    exec_name = os.path.split(exec_path)[-1]
    result_file_name = os.path.join(path_to_test_dir, f"{exec_name}.ans")

    if os.path.isfile(test_file_name):    
        exec_command = f"(/usr/bin/time -f \"%U\" {exec_path} < {test_file_name} > {result_file_name}) 2>> {result_file_name}"
        os.system(exec_command)
    
    else:
        print(f"\t-Can not generate ans because test file {test_file_name} does not exist\n"
               "\t-Run test generator first")

    
def main():
    args = parse_args()
    
    for dir_name in os.listdir():        
        if os.path.isdir(dir_name) and dir_name.startswith("test_"):
            print(f"generating answer for {dir_name}")
            generate_ans(dir_name, args.exec_path)


if __name__ == "__main__":
    main()
