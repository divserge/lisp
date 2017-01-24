#include "lispp_sexpression.h"

std::unique_ptr<SExpression> SExpression::Or(LexicalScope* scope, SExpression* expr) {
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
        throw std::runtime_error("runtime error");
    }

    SExpression* curr_level = expr;
    
    if (curr_level -> GetType() == SExpression::Type::EMPTY) {
        return std::unique_ptr<SExpression> (new SExpression(std::string("#f")));
    } else {
        if (curr_level -> Car() == nullptr) {
            throw std::runtime_error("runtime error");
        } else {
            curr_level -> Car() -> Evaluate(scope);
            if (curr_level -> Car() -> BooleanValue()) {
                return curr_level -> moveCar();
            }
        }
    } 

    while (curr_level -> Cdr() -> GetType() != SExpression::Type::EMPTY) {
        if (curr_level -> Car() == nullptr) {
            throw std::runtime_error("runtime error");
        } else {
            curr_level -> Car() -> Evaluate(scope);
            if (curr_level -> Car() -> BooleanValue()) {
                return curr_level -> moveCar(); 
            }
        }
        curr_level = curr_level -> Cdr();
    }
    curr_level -> Car() -> Evaluate(scope);
    if (curr_level -> Car() -> GetType() == SExpression::Type::EMPTY) {
        return std::unique_ptr<SExpression> (new SExpression(std::string("#f")));
    } else {
        return curr_level -> moveCar();    
    }
}

std::unique_ptr<SExpression> SExpression::And(LexicalScope* scope, SExpression* expr) {
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
        throw std::runtime_error("Runtime error : And - statement :: Ill-formed dotted list");
    }

    SExpression* curr_level = expr;
    
    if (curr_level -> GetType() == SExpression::Type::EMPTY) {
        return std::unique_ptr<SExpression> (new SExpression(std::string("#t")));
    } else {
        if (curr_level -> Car() == nullptr) {
            throw std::runtime_error("Runtime error: And - statement :: Invalid argument");
        } else {
            curr_level -> Car() -> Evaluate(scope);
            if (!(curr_level -> Car() -> BooleanValue())) {
                return curr_level -> moveCar();
            }
        }
    } 

    while (curr_level -> Cdr() -> GetType() != SExpression::Type::EMPTY) {
        
        SExpression* summand = nullptr;
        
        if (curr_level -> Car() == nullptr) {
            throw std::runtime_error("runtime error");
        } else {
            curr_level -> Car() -> Evaluate(scope);
            if (!(curr_level -> Car() -> BooleanValue())) {
                
                return curr_level -> moveCar(); 
            }
        }
        curr_level = curr_level -> Cdr();
    }
    curr_level -> Car() -> Evaluate(scope);
    return curr_level -> moveCar();    
}

std::unique_ptr<SExpression> SExpression::If(LexicalScope* scope, SExpression* expr) {
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
        throw std::runtime_error("Syntax error: If statement - Ill-formed dotted list");
    }

    if (list_size != 3 && list_size != 2) {
        throw std::runtime_error("Syntax error: If statement - invalid number of arguments");
    }

    SExpression* curr_level = expr;
    SExpression* condition = expr -> Car();
    condition -> Evaluate(scope);
    if (condition -> BooleanValue()) {
        expr -> Cdr() -> Car() -> Evaluate(scope);
        return expr -> Cdr() -> moveCar();
    } else {
        if (expr -> Cdr() -> Cdr() -> Car()) {
            expr -> Cdr() -> Cdr() -> Car() -> Evaluate(scope);
            return expr -> Cdr() -> Cdr() -> moveCar();
        } else {
            return std::unique_ptr<SExpression> (new SExpression(true));
        }
    }  
}
