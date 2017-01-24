#include <memory>
#include <iostream>

#include "lispp_tokenizer.h"
#include "lispp_sexpression.h"
#include "lispp_sexpression_parser.h"

std::unique_ptr<SExpression> LispSExpressionParser::GetNewPair(bool expect_closing, bool enquoted) {
	std::unique_ptr<SExpression> expression (new SExpression());
	std::unique_ptr<SExpression> car (GetSExpression(true, enquoted));
	std::unique_ptr<SExpression> cdr;
	if (enquoted && (tokenizer_ -> HasSpace() ||
					 tokenizer_ -> GetType() == LispTokenizer::TokenType::BRACKET_CLOSED)) {
		cdr = std::unique_ptr<SExpression> (new SExpression(true));
	} else {
		cdr =  GetSExpression(false, enquoted);
	}
	if (expect_closing) {
		CloseForm();
	}

	expression.get() -> SetCar(std::move(car));
	expression.get() -> SetCdr(std::move(cdr));
	
	return expression;
}

std::unique_ptr<SExpression> LispSExpressionParser::CloseForm() {
	if (tokenizer_ -> GetType() != LispTokenizer::TokenType::BRACKET_CLOSED) {
		throw std::runtime_error("Syntax error : Attempting to close with no closing bracket");
	}
	if (bracket_stack_.empty()) {
		throw std::runtime_error("Syntax error : Ambigous closing bracket");
	}
	bracket_stack_.pop_back();
	tokenizer_ -> Consume();
	return std::unique_ptr<SExpression> (new SExpression(true));
}

std::unique_ptr<SExpression> LispSExpressionParser::GetSExpression(bool expect_datum, bool enquoted) {
	if (tokenizer_ -> GetType() == LispTokenizer::TokenType::END) {
		if (!bracket_stack_.empty()) {
			throw std::runtime_error("Syntax error : Unexpected EOF");
		} else {
			return std::unique_ptr<SExpression> (new SExpression(true));
		}
	} else if (tokenizer_ -> GetType() == LispTokenizer::TokenType::BRACKET_CLOSED) {
		if (expect_datum) {
			throw std::runtime_error("Syntax error : Expecting datum, received closing bracket");
		} else {
			return std::unique_ptr<SExpression> (new SExpression(true));
		}
	} else if (tokenizer_ -> GetType() == LispTokenizer::TokenType::BRACKET_OPEN) {
		if (!expect_datum) {
			return GetNewPair(false, false);
		}
		bracket_stack_.push_back(true);
		tokenizer_ -> Consume();
		if (tokenizer_ -> GetType() == LispTokenizer::TokenType::BRACKET_CLOSED) {
			bracket_stack_.pop_back();
			tokenizer_ -> Consume();
			return std::unique_ptr<SExpression> (new SExpression(true));
		}
		return GetNewPair(true, false);
	} else if (tokenizer_ -> GetType() == LispTokenizer::TokenType::NUMBER) {
		if (!expect_datum && !bracket_stack_.empty()) {
			return GetNewPair(false, false);
		}
		int64_t value = tokenizer_ -> GetNumber();
		tokenizer_ -> Consume();
		if (sign_ == '-') {
			value = 0 - value;
			sign_ = '+';
		}
		return std::unique_ptr<SExpression> (new SExpression(value));
	} else if (tokenizer_ -> GetType() == LispTokenizer::TokenType::SYMBOL) {
		if (!expect_datum && !bracket_stack_.empty()) {
			return GetNewPair(false, false);
		}

		char initial = tokenizer_ -> GetSymbol();
		tokenizer_ -> Consume();
		if (initial == '-' || initial == '+') {
			if (tokenizer_ -> HasSpace()) {
				std::string operation;
				operation += initial;
				return std::unique_ptr<SExpression> (new SExpression(operation));
			}
			sign_ = initial;
			if (tokenizer_ -> GetType() == LispTokenizer::TokenType::SYMBOL) {
				throw std::runtime_error("Syntax error : +/- can not serve as an initial character"); //by convention initial can not be + or -
			}
			if (tokenizer_ -> GetType() == LispTokenizer::TokenType::NUMBER) {
				return GetSExpression(true, false);
			}
		}

		std::string symbolic_expression;
		symbolic_expression += initial;
		while (!tokenizer_ -> HasSpace() && tokenizer_ -> GetType() == LispTokenizer::TokenType::SYMBOL) {
			char new_symbol = tokenizer_ -> GetSymbol();
			if (new_symbol == 32) {
				tokenizer_ -> Consume();
				break;	
			}
			symbolic_expression += tokenizer_ -> GetSymbol();
			tokenizer_ -> Consume();
		}
		return std::unique_ptr<SExpression> (new SExpression(symbolic_expression));
	} else if (tokenizer_ -> GetType() == LispTokenizer::TokenType::DOT) {
		if (expect_datum) {
			throw std::runtime_error("Syntax error : Expected datum, receive '.' ");
		} else {
			tokenizer_ -> Consume();
			return GetSExpression(true, false);
		}
	} else {
		return std::unique_ptr<SExpression> (nullptr);
	}
}

