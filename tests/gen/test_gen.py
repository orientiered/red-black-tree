import os
import random

tests_folder = ".."

query_max_freq = 10
test_per_len = 10
test_lens = [100, 1000, 10000, 100000]
unique_elem_coeff = 0.6

test_names = []

for test_len in test_lens:
    for test_index in range(test_per_len):
        test_name = f"auto_test_{test_len}_{test_index}"
        test_path = os.path.join(tests_folder, test_name + ".dat")
        test_names.append(test_name)

        unique_elems = test_len * unique_elem_coeff

        with open(test_path, "w") as test_file:
            cur_test_len = 0
            while cur_test_len < test_len:
                cur_streak_len = random.randint(0, query_max_freq)
                cur_test_len += cur_streak_len

                for i in range(cur_streak_len):
                    test_file.write(f"k {random.randint(1, unique_elems)} ")

                query_left  = random.randint(1, unique_elems)
                query_right = random.randint(query_left, unique_elems)
                test_file.write(f"q {query_left} {query_right} ")

os.system("g++ reference.cpp -o reference")

for test_name in test_names:
    test_path = os.path.join(tests_folder, test_name + ".dat")
    test_ans  = os.path.join(tests_folder, test_name + ".ans")
    os.system(f"./reference < {test_path} > {test_ans}")
