#pragma once

#include <iostream>
#include <string>

#include "lispp_tokenizer.h"
#include "lispp_sexpression.h"
#include "lispp_sexpression_parser.h"
#include "lispp_lexical_scope.h"

class LispInterpreter {
  public:
  	LispInterpreter(std::istream* input, std::ostream* output);
  	LispInterpreter();
  	~LispInterpreter();
  	void LaunchREPL();
  	std::string Evaluate(std::string expression);
  private:
  	void InitializeScope();
  	std::istream* input_;
  	std::ostream* output_;
  	LispTokenizer tokenizer_;
  	LispSExpressionParser parser_;
  	bool owns_streams_;
  	LexicalScope global_scope_;
};

//# -Wall -Wextra -Werror - enable all compiler warnings and turn them into errors