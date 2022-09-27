from argparse import ArgumentParser
import os
from random import randint


def parse_args():
    parser = ArgumentParser(description='Process test generator args')

    parser.add_argument('--test-num', type=int, nargs='?', default=10,
                        help='total num of tests, default = 10')

    parser.add_argument('--test-size', type=int, nargs='?', default=10000,
                        help='total num of requests in test, default = 10000')

    parser.add_argument('--cache-size', type=int, nargs='?', default=400,
                        help='size of the cache, default = 400')
    
    parser.add_argument('--test-lower-bound', type=int, nargs='?', default=-10000,
                        help='lower boundary of tests inputs, default = -10000')

    parser.add_argument('--test-upper-bound', type=int, nargs='?', default=10000,
                        help='upper boundary of tests inputs, default = 10000')

    return parser.parse_args()


def generate_one_test(path_to_test_dir, test_size, cache_size, test_lower_bound, test_upper_bound):
    test_file_name   = os.path.join(path_to_test_dir, "test")

    with open(test_file_name, "w") as test_file:
        print(cache_size, test_size, file=test_file)

        for _ in range (test_size):
            print(randint(test_lower_bound, test_upper_bound), file=test_file)
    
        
def main():
    args = parse_args()
    
    for idx in range(1, args.test_num + 1):
        dir_name = f"test_{idx}"
        
        if not os.path.exists(dir_name):
            os.makedirs(dir_name)
        
        generate_one_test(dir_name, args.test_size, args.cache_size, 
                          args.test_lower_bound, args.test_upper_bound)

if __name__ == "__main__":
    main()
