#pragma once

#include "lispp_sexpression.h"
#include "lispp_tokenizer.h"

#include <memory>
#include <vector>

class LispSExpressionParser {
  public:
  	LispSExpressionParser (LispTokenizer* tokenizer) : tokenizer_ (tokenizer) {}
  	std::unique_ptr<SExpression> GetSExpression(bool expect_datum, bool enquoted);
  private:
  	LispTokenizer* tokenizer_;
	std::unique_ptr<SExpression> GetSExpression() { return GetSExpression(true, false); }
  	std::unique_ptr<SExpression> GetNewPair(bool expect_closing, bool enquoted);
  	std::unique_ptr<SExpression> CloseForm();
  	std::vector<bool> bracket_stack_;
  	
  	char sign_ = '+';
    bool enquoted_ = false;
};
