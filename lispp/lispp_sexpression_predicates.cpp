#include "lispp_sexpression.h"

std::unique_ptr<SExpression> SExpression::IsList(LexicalScope* scope, SExpression* expr) {
    int64_t list_size = 0;
    SExpression* list_validity_check = expr;
    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        ++list_size;
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_size != 1) {
        throw std::runtime_error("Runtime error : list? invalid number of arguments");
    }

    if (list_validity_check == nullptr) {
        throw std::runtime_error("Runtime error : list? not an argument list");
    }

    list_validity_check = expr -> Car();
    expr -> Car() -> Evaluate(scope);

    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        ++list_size;
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_validity_check == nullptr) {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, false));
    } else {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, true));
    }

}

std::unique_ptr<SExpression> SExpression::IsNumber(LexicalScope* scope, SExpression* expr) {
    size_t list_size = 0;
    SExpression* list_validity_check = expr;
    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        ++list_size;
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_size != 1) {
        throw std::runtime_error("Runtime error : number? invalid number of arguments");
    }

    if (list_validity_check == nullptr) {
        throw std::runtime_error("Runtime error : number? not an argument list");
    }

    list_validity_check = expr -> Car();
    expr -> Car() -> Evaluate(scope);
    if (expr -> Car() -> GetType() != SExpression::Type::NUMBER) {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, false));
    } else {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, true));
    }
}

std::unique_ptr<SExpression> SExpression::IsPair(LexicalScope* scope, SExpression* expr) {
    size_t list_size = 0;
    SExpression* list_validity_check = expr;
    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        ++list_size;
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_size != 1) {
        throw std::runtime_error("Runtime error : pair? invalid number of arguments");
    }

    if (list_validity_check == nullptr) {
        throw std::runtime_error("Runtime error : pair? not an argument list");;
    }

    list_validity_check = expr -> Car();
    expr -> Car() -> Evaluate(scope);
    if (expr -> Car() -> GetType() != SExpression::Type::EXPRESSION) {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, false));
    } else {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, true));
    }
}

std::unique_ptr<SExpression> SExpression::IsNull(LexicalScope* scope, SExpression* expr) {
    size_t list_size = 0;
    SExpression* list_validity_check = expr;
    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        ++list_size;
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_validity_check == nullptr) {
        throw std::runtime_error("Runtime error : null? not an argument list");
    }

    list_validity_check = expr -> Car();
    expr -> Car() -> Evaluate(scope);
    if (expr -> Car() -> GetType() != SExpression::Type::EMPTY) {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, false));
    } else {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, true));
    }
}

std::unique_ptr<SExpression> SExpression::IsSymbol(LexicalScope* scope, SExpression* expr) {
    size_t list_size = 0;
    SExpression* list_validity_check = expr;
    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        ++list_size;
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_size != 1) {
        throw std::runtime_error("Runtime error : symbol? invalid number of arguments");
    }

    if (list_validity_check == nullptr) {
        throw std::runtime_error("Runtime error : symbol? not an argument list");
    }

    list_validity_check = expr -> Car();
    expr -> Car() -> Evaluate(scope);
    if (expr -> Car() -> GetType() != SExpression::Type::SYMBOL) {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, false));
    } else {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, true));
    }
}

std::unique_ptr<SExpression> SExpression::IsBool(LexicalScope* scope, SExpression* expr) {
    size_t list_size = 0;
    SExpression* list_validity_check = expr;
    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        ++list_size;
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_size != 1) {
        throw std::runtime_error("Runtime error : bool? invalid number of arguments");
    }

    if (list_validity_check == nullptr) {
        throw std::runtime_error("Runtime error : bool? not an argument list");
    }

    list_validity_check = expr -> Car();
    expr -> Car() -> Evaluate(scope);
    if (expr -> Car() -> GetType() != SExpression::Type::BOOL) {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, false));
    } else {
        return std::unique_ptr<SExpression>(new SExpression(SExpression::Type::BOOL, true));
    }
}
