#pragma once

#include <string>
#include <sstream>
#include <stdexcept>

#include <lispp/lispp_interpreter.h>

#include <gtest/gtest.h>

struct LispTest : public ::testing::Test {
    // initialize new interpreter here

    LispInterpreter interpreter;
    // evaluate expression and compare result to expected
    void ExpectEq(std::string expression, std::string result) {
        ASSERT_EQ(result, interpreter.Evaluate(expression));
    }

    // evaluate expression and check that there is not errors
    void ExpectNoError(std::string expression) {
        try {
            interpreter.Evaluate(expression);
        } catch (std::runtime_error& error) {
            FAIL() << "Exception thrown with a correct imput" << std::endl;
        }
    }

    // check that expression evaluation produces syntax error
    void ExpectSyntaxError(std::string expression) {
        interpreter.Evaluate(expression);
        ASSERT_EQ("Syntax error", interpreter.Evaluate(expression));
    }

    // check that expression evaluation produces runtime error
    void ExpectRuntimeError(std::string expression) {
        interpreter.Evaluate(expression);
        ASSERT_EQ("Runtime error", interpreter.Evaluate(expression));
    }

    void ExpectUndefinedVariable(std::string expression) {
        ASSERT_EQ("name error", interpreter.Evaluate(expression));
    }
};