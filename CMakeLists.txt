cmake_minimum_required(VERSION 3.16)
project(RBTree)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

set(tree_exec_name      rbtree)
set(unit_test_exec_name tree_test)
set(benchmark_exec_name tree_bench)

# ----------------------- Dependencies ---------------------------- #

add_subdirectory(googletest)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests" FORCE)
add_subdirectory(benchmark)

# ----------------------- Executables ----------------------------- #

# Main executable
add_executable(${tree_exec_name} src/main.cpp)
target_include_directories(${tree_exec_name} PUBLIC include)
target_include_directories(${tree_exec_name} PUBLIC googletest/googletest/include)

SET(ASAN_FLAGS -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
SET(WARNING_FLAGS -Wextra -Weffc++ -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector)

target_compile_options(${tree_exec_name} PUBLIC $<$<CONFIG:Debug>:-g ${ASAN_FLAGS}> ${WARNING_FLAGS})
target_link_options(${tree_exec_name} PRIVATE $<$<CONFIG:Debug>:${ASAN_FLAGS}>)

# Rbtree unit testing
add_executable(${unit_test_exec_name} tests/unit/tree_test.cpp)
target_include_directories(${unit_test_exec_name} PUBLIC include)
target_include_directories(${unit_test_exec_name} PUBLIC googletest/googletest/include)
target_link_libraries(${unit_test_exec_name} gtest_main)

# Target for benchmarking executable
add_executable(${benchmark_exec_name} tests/bench/tree_bench.cpp)
target_include_directories(${benchmark_exec_name} PUBLIC include)
target_include_directories(${benchmark_exec_name} PUBLIC benchmark/include)
target_include_directories(${benchmark_exec_name} PUBLIC googletest/googletest/include)
target_link_libraries(${benchmark_exec_name} benchmark_main)

# Std set implementation for test generation
add_executable(reference tests/end2end/gen/reference.cpp)

add_custom_command(
        TARGET reference POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${PROJECT_BINARY_DIR}/reference
                ${CMAKE_SOURCE_DIR}/tests/end2end/gen/reference
)
# ----------------------- Testing ----------------------------- #

enable_testing()

include(GoogleTest)
gtest_discover_tests(${unit_test_exec_name}
    PROPERTIES
        LABELS "unit"
)

SET(END2END_TESTS_DIR ${CMAKE_SOURCE_DIR}/tests/end2end)
file(GLOB testFiles ${END2END_TESTS_DIR}/*.dat)

foreach(file ${testFiles})
    get_filename_component(test_base_name ${file} NAME_WLE)
    set(test_name "End2End.${test_base_name}")
    add_test(NAME ${test_name}
             COMMAND "${CMAKE_SOURCE_DIR}/run_test.sh"
                     "./${tree_exec_name}"
                     "${file}"
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    set_tests_properties(${test_name}
        PROPERTIES
            DEPENDS ${tree_exec_name}
            LABELS "end2end"
    )
endforeach()

# ----------------------- Benchmark targets ----------------------------- #

SET(bench_test_list ${CMAKE_SOURCE_DIR}/tests/bench/bench_test_list.txt)
SET(bench_result_dir ${CMAKE_SOURCE_DIR}/tests/bench/results )
SET(compare_script ${CMAKE_SOURCE_DIR}/benchmark/tools/compare.py)

add_custom_target(bench_stdset
    COMMAND RBTREE_BENCH_TYPE=STD_SET RBTREE_BENCH_PATH_PREFIX=${CMAKE_SOURCE_DIR} RBTREE_BENCH_TEST_LIST=${bench_test_list} ${PROJECT_BINARY_DIR}/${benchmark_exec_name} --benchmark_time_unit=ms
    DEPENDS ${benchmark_exec_name}
)

add_custom_target(bench_rbtree
    COMMAND RBTREE_BENCH_TYPE=RB_TREE RBTREE_BENCH_PATH_PREFIX=${CMAKE_SOURCE_DIR} RBTREE_BENCH_TEST_LIST=${bench_test_list} ${PROJECT_BINARY_DIR}/${benchmark_exec_name} --benchmark_time_unit=ms
    DEPENDS ${benchmark_exec_name}
)

add_custom_target(bench
    COMMAND mkdir -p ${bench_result_dir}
    COMMAND RBTREE_BENCH_TYPE=RB_TREE BENCHMARK_OUT=${bench_result_dir}/rbtree_result.json RBTREE_BENCH_PATH_PREFIX=${CMAKE_SOURCE_DIR} RBTREE_BENCH_TEST_LIST=${bench_test_list} ${PROJECT_BINARY_DIR}/${benchmark_exec_name} --benchmark_time_unit=ms
    COMMAND RBTREE_BENCH_TYPE=STD_SET BENCHMARK_OUT=${bench_result_dir}/stdset_result.json RBTREE_BENCH_PATH_PREFIX=${CMAKE_SOURCE_DIR} RBTREE_BENCH_TEST_LIST=${bench_test_list} ${PROJECT_BINARY_DIR}/${benchmark_exec_name} --benchmark_time_unit=ms
    COMMAND python3 ${compare_script} benchmarks ${bench_result_dir}/stdset_result.json ${bench_result_dir}/rbtree_result.json
    DEPENDS ${benchmark_exec_name}
)

