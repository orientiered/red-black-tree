# Red-black tree

Red-black tree implementation in C++.

## Build

```bash
git submodule update --init
cmake -B build
cmake --build build
```

### Dependencies

For benchmarking (`bench` CMake target) you may need to install `numpy` and `scipy`:
```bash
pip install numpy scipy
```

## Using

__Input format__: sequence of insert or range queries with form _"k &lt;int&gt;"_ or _"q &lt;int&gt; &lt;int&gt;"_ respectively.
__Output format__: one integer for each query - number of integers between two given bounds: _(left, right]_.

_Example_:
```bash
./build/rbtree
k 40 k 14 k 35 q 35 40 q 1 3 k 49 k 17 k 33 q 31 38 k 33 k 31 k 6 k 23 q 48 59
1 0 2 1
```

## Testing

The program can be tested using __ctest__ tool. There are two types of tests: end-to-end and unit. You can run both:

```bash
cd build

# all tests
ctest

# only unit
ctest -L unit

# only end-to-end
ctest -L end2end
```

## Benchmarking

Our tree implementation can be benchmarked against `std::set` with **google benchmark** using cmake targets `bench_rbtree`, `bench_stdset` and `bench` for comparison:
```bash
cmake --build build --target bench
# run both benchmarks and compare results
```

Benchmark data files are taken from [`bench_test_list.txt`](/tests/bench/bench_test_list.txt)

Output example:
```
Implementation: std::set
-----------------------------------------------------
Benchmark           Time             CPU   Iterations
-----------------------------------------------------
100k 1           1006 ms          987 ms            1
100k 2            987 ms          969 ms            1
100k 3            947 ms          932 ms            1
100k 4            957 ms          944 ms            1
10k 1            4.91 ms         4.85 ms          137
10k 2            4.91 ms         4.86 ms          139
10k 3            5.06 ms         5.01 ms          132
10k 4            4.78 ms         4.74 ms          147
```
