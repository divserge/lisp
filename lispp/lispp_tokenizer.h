#pragma once

#include <iostream>
#include <vector>

class LispTokenizer {
  public:
    LispTokenizer(std::istream* in): in_(in) {}
    enum TokenType {
        BRACKET_OPEN,
        BRACKET_CLOSED,
        QUOTE,
        DOT,
        NUMBER,
        SYMBOL,
        EMPTY,
        UNKNOWN,
        END
    };
    bool HasSpace();
    void Consume();
    TokenType GetType();
    int64_t GetNumber();
    char GetSymbol();

  private:
    TokenType type_ = TokenType::UNKNOWN;
    int64_t number_;
    bool negative_ = false;
    char symbol_;

    std::string quote_buffer_;
    std::vector<bool> quote_brackets_stack_;
    bool quoted_ = false;
    bool close_quote_ = false;

    std::istream* in_;
    void EnsureRead();
};
