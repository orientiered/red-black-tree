cmake_minimum_required(VERSION 3.16)
project(RBTree)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

set(tree_exec_name      rbtree)
set(unit_test_exec_name tree_test)

add_subdirectory(googletest)

add_executable(${tree_exec_name} src/main.cpp)
target_include_directories(${tree_exec_name} PUBLIC include)
target_include_directories(${tree_exec_name} PUBLIC googletest/googletest/include)

add_executable(${unit_test_exec_name} tests/unit/tree_test.cpp)
target_include_directories(${unit_test_exec_name} PUBLIC include)
target_include_directories(${unit_test_exec_name} PUBLIC googletest/googletest/include)
target_link_libraries(${unit_test_exec_name} gtest_main)

enable_testing()

SET(ASAN_FLAGS -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
SET(WARNING_FLAGS -Wextra -Weffc++ -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector)

target_compile_options(${tree_exec_name} PUBLIC $<$<CONFIG:Debug>:-g ${ASAN_FLAGS}> ${WARNING_FLAGS})
target_link_options(${tree_exec_name} PRIVATE $<$<CONFIG:Debug>:${ASAN_FLAGS}>)

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
