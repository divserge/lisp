#include "lispp_sexpression.h"
#include "lispp_lexical_scope.h"

std::unique_ptr<SExpression> SExpression::Set(LexicalScope* scope, SExpression* expr) {
    SExpression* list_validity_check = expr;

    size_t list_size = 0;
    
    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        ++list_size;
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_validity_check == nullptr) {
        throw std::runtime_error("Syntax error : Set - Ill-formed dotted list");
    }

    if (list_size != 2) {
        throw std::runtime_error("Syntax error : Set - Invalid number of arguments");
    }

    SExpression* curr_level = expr;

    if (expr -> Car() -> GetType() != SYMBOL) {
        throw std::runtime_error("runtime error");
    } 
    
    curr_level -> Cdr() -> Car() -> Evaluate(scope);

    if (scope -> LookUp(curr_level -> Car() -> SymbolicValue()) == nullptr) {
        throw std::runtime_error("Undefined variable : Set");
    }

    scope -> setVariable(curr_level -> Car() -> SymbolicValue(), 
                         std::move(curr_level -> Cdr() -> moveCar()));
    curr_level -> Car() -> EnQuote();
    if (curr_level -> Car() -> GetType() == SYMBOL) {
    }
    return curr_level -> moveCar();
}

std::unique_ptr<SExpression> SExpression::Define(LexicalScope* scope, SExpression* expr) {
    SExpression* list_validity_check = expr;

    size_t list_size = 0;
    
    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        ++list_size;
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_validity_check == nullptr) {
        throw std::runtime_error("Syntax error : Define - Ill-formed dotted list");
    }

    if (list_size != 2) {
        throw std::runtime_error("Syntax error : Define - requires two arguments");
    }

    SExpression* curr_level = expr;

    if (expr -> Car() -> GetType() == EXPRESSION) {
        //defining lambda
        std::unique_ptr<SExpression> arguments = expr -> Car() -> moveCdr();
        std::unique_ptr<SExpression> name = expr -> Car() -> moveCar();
        std::unique_ptr<SExpression> function = expr -> moveCdr();
        std::unique_ptr<SExpression> lambda (new SExpression);
        lambda.get() -> car_ = std::move(arguments);
        lambda.get() -> cdr_ = std::move(function);
        std::unique_ptr<SExpression> lambda_defined = LambdaCreate(scope, lambda.get());
        scope -> setVariable(name.get() -> SymbolicValue(), std::move(lambda_defined));
        return std::move(name);
    }

    if (expr -> Car() -> GetType() != SYMBOL) {
        throw std::runtime_error("runtime error");
    } 

    scope -> setVariable(curr_level -> Car() -> SymbolicValue(), 
                         std::move(curr_level -> Cdr() -> moveCar()));
    curr_level -> Car() -> EnQuote();
    if (curr_level -> Car() -> GetType() == SYMBOL) {
    }
    return curr_level -> moveCar();
} 
