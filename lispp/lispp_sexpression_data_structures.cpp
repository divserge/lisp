#include "lispp_sexpression.h"
#include "lispp_lexical_scope.h"

std::unique_ptr<SExpression> SExpression::Cons(LexicalScope* scope, SExpression* expr)  {
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
        throw std::runtime_error("Runtime error :: Cons - Ill-formed dotted list");
    }

    if (list_size != 2) {
        throw std::runtime_error("Runtime error:: Cons - invalid number of arguments");
    }

    return std::unique_ptr<SExpression> (new SExpression(expr -> moveCar(), expr -> Cdr() -> moveCar(),
                                                         SExpression::Type::EXPRESSION));
}

std::unique_ptr<SExpression> SExpression::List(LexicalScope* scope, SExpression* expr) {
    SExpression* list_validity_check = expr;
    
    if (expr -> GetType() == SExpression::Type::EMPTY) {
        return std::unique_ptr<SExpression> (new SExpression(true));
    }


    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_validity_check == nullptr) {
        throw std::runtime_error("Runtime error :: List - Ill-formed dotted list");
    }

    return std::unique_ptr<SExpression> (new SExpression(expr -> moveCar(), expr -> moveCdr(),
                                                         SExpression::Type::EXPRESSION));
}

std::unique_ptr<SExpression> SExpression::SetCar(LexicalScope* scope, SExpression* expr) {
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

    if (list_size != 2) {
        throw std::runtime_error("runtime error");
    }

    SExpression* curr_level = expr;
    SExpression* setting = expr -> Car();
    
    if (setting -> GetType() == SYMBOL) {
            setting = scope -> LookUp(setting -> SymbolicValue());
    }

    if (setting -> GetType() != EXPRESSION) {
        throw std::runtime_error("runtime error");
    }

    SExpression* new_value = expr -> Cdr() -> Car();
    new_value -> Evaluate(scope);
    bool need_to_quote = setting -> Car() -> isQuoted();
    *(setting->Cdr() -> Car() -> Car()) = std::move(*new_value);
    setting -> EnQuote();
    return nullptr;
}

std::unique_ptr<SExpression> SExpression::SetCdr(LexicalScope* scope, SExpression* expr) {
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

    if (list_size != 2) {
        throw std::runtime_error("runtime error");
    }

    SExpression* curr_level = expr;
    SExpression* setting = expr -> Car();
    
    if (setting -> GetType() == SYMBOL) {
            setting = scope -> LookUp(setting -> SymbolicValue());
    }

    if (setting -> GetType() != EXPRESSION) {
        throw std::runtime_error("runtime error");
    }

    SExpression* new_value = expr -> Cdr() -> Car();
    new_value -> Evaluate(scope);
    bool need_to_quote = setting -> Car() -> isQuoted();
    *(setting->Cdr() -> Car() -> Cdr()) = std::move(*new_value);
    setting -> EnQuote();
    return nullptr;
}

std::unique_ptr<SExpression> SExpression::GetCar(LexicalScope* scope, SExpression* expr) {
    expr -> Car() -> Evaluate(scope);
    if (expr -> Car() -> GetType() != EXPRESSION) {
        throw std::runtime_error("runtime error");
    } else {
        return expr -> Car() -> moveCar();
    }
}

std::unique_ptr<SExpression> SExpression::GetCdr(LexicalScope* scope, SExpression* expr) {
    expr -> Car() -> Evaluate(scope);
    if (expr -> Car() -> GetType() != EXPRESSION) {
        throw std::runtime_error("runtime error");
    } else {
        return expr -> Car() -> moveCdr();
    }
}

std::unique_ptr<SExpression> SExpression::ListTail(LexicalScope* scope, SExpression* expr) {
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

    if (list_size != 2) {
        throw std::runtime_error("runtime error");
    }

    SExpression* index = expr -> Cdr() -> Car();
    index -> Evaluate(scope);

    if (index -> GetType() != NUMBER) {
        throw std::runtime_error("runtime error");
    } 
    
    SExpression* list = expr -> Car();
    list -> Evaluate(scope);
    list_validity_check = list;
    list_size = 0;
    
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

    if (list_size < index -> NumericValue()) {
        throw std::runtime_error("runtime error");
    }

    if (index -> NumericValue() == 0) {
        return expr -> moveCar();
    }

    size_t counter = 0;

    while (counter < index -> NumericValue() - 1) {
        list = list -> Cdr();
        ++counter;
    }

    return list -> moveCdr();
}

std::unique_ptr<SExpression> SExpression::ListRef(LexicalScope* scope, SExpression* expr) {
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
        throw std::runtime_error("Runtime error : ListRef - Ill-formed dotted list");
    }

    if (list_size != 2) {
        throw std::runtime_error("Runtime error : ListRef - wrong number of arguments");
    }

    SExpression* index = expr -> Cdr() -> Car();
    index -> Evaluate(scope);

    if (index -> GetType() != NUMBER) {
        throw std::runtime_error("Runtime error : ListRef - second argument must be of a type number");
    } 
    
    SExpression* list = expr -> Car();
    list -> Evaluate(scope);
    list_validity_check = list;
    list_size = 0;
    
    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        ++list_size;
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_validity_check == nullptr) {
        throw std::runtime_error("ListRef - ListRef - argument not a list");
    }

    if (list_size <= index -> NumericValue()) {
        throw std::runtime_error("Runtime error : ListRef - index out of bounds");
    }

    size_t counter = 0;

    while (counter < index -> NumericValue()) {
        list = list -> Cdr();
        ++counter;
    }

    return list -> moveCar();
}
