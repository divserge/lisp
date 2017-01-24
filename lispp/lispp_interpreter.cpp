/*#include "lispp_tokenizer.h"
#include "lispp_scope.h"
#include "s_expression.h"*/

#include "lispp_interpreter.h"
#include "lispp_lexical_scope.h"
#include "lispp_sexpression.h"
#include "lispp_sexpression_parser.h"

#include <iostream>
#include <string>
#include <sstream>

LispInterpreter::LispInterpreter() : input_(new std::istringstream),
									 output_(new std::ostringstream),
									 owns_streams_(true),
									 tokenizer_(input_),
									 parser_(&tokenizer_) {
	InitializeScope();
}
LispInterpreter::LispInterpreter(std::istream* input, std::ostream* output) : input_(input),
  																 tokenizer_ (input),
  																 owns_streams_(false),
  																 parser_ (&tokenizer_),
  																 output_ (output) {
  	InitializeScope();
}

LispInterpreter::~LispInterpreter() {
	if (owns_streams_) {
		delete input_;
		delete output_;
	}
}

void LispInterpreter::InitializeScope() {
	global_scope_.setVariable("+", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::SUM)));
    global_scope_.setVariable("*", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::MULT)));
    global_scope_.setVariable("or", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::OR)));
    global_scope_.setVariable("and", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::AND)));
    global_scope_.setVariable("if", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::IF)));
    global_scope_.setVariable("define", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::DEFINE)));
    global_scope_.setVariable("list-ref", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::LISTREF)));
    global_scope_.setVariable("quote", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::QUOTE)));
    global_scope_.setVariable("list-tail", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::LISTTAIL)));
    global_scope_.setVariable("car", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::CAR)));
    global_scope_.setVariable("cdr", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::CDR)));
    global_scope_.setVariable("set-car!", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::SETCAR)));
    global_scope_.setVariable("set-cdr!", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::SETCDR)));
    global_scope_.setVariable("lambda", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::LAMBDACREATE)));
    global_scope_.setVariable("set!", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::SET)));
    global_scope_.setVariable("=", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::INTEQUAL)));
    global_scope_.setVariable("<", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::INTLESS)));
    global_scope_.setVariable(">=", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::GEQ)));
    global_scope_.setVariable("<=", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::LEQ)));
    global_scope_.setVariable(">", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::INTGREATER)));
    global_scope_.setVariable("-", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::DEC)));
    global_scope_.setVariable("list?", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::ISLIST)));
    global_scope_.setVariable("number?", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::ISNUM)));
    global_scope_.setVariable("symbol?", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::ISSYM)));
    global_scope_.setVariable("boolean?", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::ISBOOL)));
    global_scope_.setVariable("null?", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::ISNULL)));
    global_scope_.setVariable("pair?", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::ISPAIR)));
    global_scope_.setVariable("abs", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::ABS)));
    global_scope_.setVariable("/", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::FRAC)));
    global_scope_.setVariable("max", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::MAX)));
    global_scope_.setVariable("min", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::MIN)));
    global_scope_.setVariable("cons", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::CONS)));
    global_scope_.setVariable("not", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::NOT)));
    global_scope_.setVariable("list", std::unique_ptr<SExpression> (new SExpression(SExpression::ApplicationType::LIST)));
}

std::string LispInterpreter::Evaluate(std::string expression) {
	std::istringstream sstream (expression);
	tokenizer_ = LispTokenizer(&sstream);
	parser_ = LispSExpressionParser(&tokenizer_);
	std::unique_ptr<SExpression> s_expression;
	try {
		s_expression = parser_.GetSExpression(true, false);
	} catch (...) {
		return "Syntax error";
	}
    if (s_expression.get() == nullptr) {
        return "";
    }
    try {
    	s_expression.get() -> Evaluate(&global_scope_);
    } catch (std::runtime_error& error) {
    	if (error.what()[0] == 'S') {
    		return "Syntax error";
    	} else if (error.what()[0] == 'U') {
    		return "name error";
    	} else {
    		return "Runtime error";
    	}
    }
    return s_expression.get() -> Print(true);
}

void LispInterpreter::LaunchREPL() {
    if (owns_streams_) {
        return;
    }
	while (tokenizer_.GetType() != LispTokenizer::TokenType::END) {
		std::unique_ptr<SExpression> s_expression;
		try {
			s_expression = parser_.GetSExpression(true, false);
		} catch (...) {
			*output_ << "syntax error" << std::endl;
			break;
		}
		if (s_expression.get() == nullptr) {
		    return;
		}
		try {
			s_expression.get() -> Evaluate(&global_scope_);
		} catch (std::runtime_error& error) {
			if (error.what()[0] == 'S') {
				*output_ <<  "syntax error" << std::endl;
				break;
			} else if (error.what()[0] == 'U') {
				*output_ <<  "name error" << std::endl;
			} else {
				*output_ <<  "runtime error" << std::endl;
			}
			continue;
		}
        *output_ << s_expression.get() -> Print(true) << std::endl;;
    }
}
