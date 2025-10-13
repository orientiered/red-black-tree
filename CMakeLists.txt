cmake_minimum_required(VERSION 3.16)
project(RBTree)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(googletest)

add_executable(rbtree src/main.cpp)

target_include_directories(rbtree PUBLIC include)
target_include_directories(rbtree PUBLIC googletest/googletest/include)

add_executable(tree_test src/tree_test.cpp)
target_include_directories(tree_test PUBLIC include)
target_include_directories(tree_test PUBLIC googletest/googletest/include)
target_link_libraries(tree_test gtest_main)

enable_testing()

SET(ASAN_FLAGS -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
SET(WARNING_FLAGS -Wextra -Weffc++ -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector)

target_compile_options(rbtree PUBLIC $<$<CONFIG:Debug>:-g ${ASAN_FLAGS}> ${WARNING_FLAGS})
target_link_options(rbtree PRIVATE ${ASAN_FLAGS})

SET(TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)
file(GLOB testFiles ${TESTS_DIR}/*.dat)

foreach(file ${testFiles})
    get_filename_component(test_name ${file} NAME_WLE)
    add_test(NAME ${test_name}
             COMMAND "${CMAKE_SOURCE_DIR}/run_test.sh"
                     "./rbtree"
                     "${file}"
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_tests_properties(${test_name} PROPERTIES DEPENDS rbtree)
endforeach()



