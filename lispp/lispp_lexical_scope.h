#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class SExpression;

class LexicalScope {
  public:
  	LexicalScope() : parent_scope_ (nullptr) {}
  	LexicalScope(LexicalScope* parent_scope);
  	void setVariable(std::string name, std::unique_ptr<SExpression>&& expression);
  	SExpression* LookUp(std::string name);
  	bool IsInitiated() { return initiated_ ;}
  	void Initiate() { initiated_ = true ; }
  private:
  	LexicalScope* parent_scope_;
  	std::unordered_map<std::string, std::unique_ptr<SExpression>> scope_;
  	bool initiated_ = false;
};
