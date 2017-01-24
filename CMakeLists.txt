project(lispp)

cmake_minimum_required(VERSION 2.8)

add_library(gtest
    test/gmock/gmock-gtest-all.cc
    test/gmock/gmock_main.cc)



# -fsanitize=undefined -fsanitize=address - detect UB and memory errors in runtime
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -g -fsanitize=undefined -fsanitize=address")

include_directories(.)
include_directories(test/gmock)

add_library(lispp
    lispp/lispp_lexical_scope.cpp
    lispp/lispp_sexpression.cpp
    lispp/lispp_sexpression_predicates.cpp
    lispp/lispp_sexpression_integer_operations.cpp
    lispp/lispp_sexpression_data_structures.cpp
    lispp/lispp_sexpression_scope.cpp
    lispp/lispp_sexpression_logic.cpp
    lispp/lispp_sexpression_lambda.cpp
    lispp/lispp_tokenizer.cpp
    lispp/lispp_sexpression_parser.cpp
    lispp/lispp_interpreter.cpp)

add_executable(lisp_repl main.cpp)

target_link_libraries(lisp_repl lispp)

add_executable(run_tests
    test/lisp_test.cpp
    test/test_eval.cpp
    test/test_integer.cpp
    test/test_boolean.cpp
    test/test_symbol.cpp
    test/test_list.cpp
    test/test_lambda.cpp
    test/test_control_flow.cpp
)

target_link_libraries(run_tests gtest pthread lispp)
