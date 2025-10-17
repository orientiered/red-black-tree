# Red-black tree

Red-black tree implementation in C++.

## Build

```bash
git submodule update --init
cmake -B build
cmake --build build
```

## Using

__Input format__: sequence of insert or range queries with form _"k &lt;int&gt;"_ or _"q &lt;int&gt; &lt;int&gt;"_ respectively.
__Output format__: one integer for each query - number of integers between two given bounds (including left, excluding right).

_Example_:
```bash
./build/rbtree
k 40 k 14 k 35 q 35 40 q 1 3 k 49 k 17 k 33 q 31 38 k 33 k 31 k 6 k 23 q 48 59
2 0 2 1
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
