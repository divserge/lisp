#include "lispp_lexical_scope.h"
#include "lispp_sexpression.h"

LexicalScope::LexicalScope(LexicalScope* parent_scope) : parent_scope_ (parent_scope) {}

void LexicalScope::setVariable(std::string name, std::unique_ptr<SExpression>&& expression) {
	if (!initiated_) {
		initiated_ = true;
	}
	scope_[name] = std::move(expression);
}

SExpression* LexicalScope::LookUp(std::string name) {
	if (scope_.find(name) == scope_.end()) {
		if (parent_scope_ != nullptr) {
			return parent_scope_ -> LookUp(name);
		} else {
			return nullptr;
		}
	} else {
		return scope_[name].get();
	}
}
