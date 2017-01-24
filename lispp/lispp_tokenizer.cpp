#include <stdexcept>
#include <iostream>

#include "lispp_tokenizer.h"

void LispTokenizer::Consume() {
    type_ = TokenType::UNKNOWN;
}
    
LispTokenizer::TokenType LispTokenizer::GetType() {
    EnsureRead();
    return type_;
}

int64_t LispTokenizer::GetNumber() {
    EnsureRead();
    if (negative_) {
        negative_ = false;
        return 0 - number_;
    } else {
        return number_;
    }
}

bool LispTokenizer::HasSpace() {
    return(isspace(in_ -> peek()));
}

char LispTokenizer::GetSymbol() {
    EnsureRead();
    return symbol_;
}

void LispTokenizer::EnsureRead() {

    if (type_ != TokenType::UNKNOWN) {
        return;
    }
    
    if (!quote_buffer_.empty()) {    //a hack to make ' syntax work properly without complicating the parser
        type_ = SYMBOL;
        symbol_ = quote_buffer_.back();
        quote_buffer_.pop_back();

    }

    if (close_quote_) {
        close_quote_ = false;
        quoted_ = false;
        type_ = BRACKET_CLOSED;
    }

    if (type_ == TokenType::UNKNOWN) {
        while (in_ -> peek() != EOF && (isspace(in_ -> peek()) || in_ -> peek() == '\n')) {
            in_ -> get();
        }
        if (in_ -> peek() == EOF) {
            type_ = END; 
            in_ -> get();
        } else if (in_ -> peek() == '(') {
            if (quoted_) {
                quote_brackets_stack_.push_back(true);
            }
            type_ = BRACKET_OPEN;
            in_ -> get();
        } else if (in_ -> peek() == ')') {
            if (quoted_) {
                if (!quote_brackets_stack_.empty()) {
                    quote_brackets_stack_.pop_back();
                }
                if (quote_brackets_stack_.empty()) {
                    close_quote_ = true;
                }
            }
            type_ = BRACKET_CLOSED;
            in_ -> get();
        } else if (in_ -> peek() == '.') {
            type_ = DOT;
            in_ -> get();
        } else if (in_ -> peek() == 39) {
            type_ = BRACKET_OPEN;
            quote_buffer_ = " etouq";
            quoted_ = true;
            in_ -> get();
            return;
        } else if (isdigit(in_ -> peek())) {
            type_ = NUMBER;
            (*in_) >> number_;
        } else {
            type_ = SYMBOL;
            (*in_) >> symbol_;
        }
        if (quoted_ && !close_quote_ && quote_brackets_stack_.empty() && (isspace(in_ -> peek()) || in_ -> peek() == EOF)) {
            close_quote_ = true;
        }

    }
}
