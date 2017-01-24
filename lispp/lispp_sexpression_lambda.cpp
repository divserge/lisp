#include "lispp_sexpression.h"
#include "lispp_lexical_scope.h"

std::unique_ptr<SExpression> SExpression::Not(LexicalScope* scope, SExpression* expr) {
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
        throw std::runtime_error("Runtime error :: Not - Ill-formed dotted list");
    }
    if (list_size != 1) {
        throw std::runtime_error("Runtime error:: Not - invalid number of arguments");
    }
    expr -> Car() -> Evaluate(scope);
    std::string bool_value;
    if (expr -> Car() -> BooleanValue()) {
        bool_value = "#f";
    } else {
        bool_value = "#t";
    }
    return std::unique_ptr<SExpression> (new SExpression(bool_value));
}

std::unique_ptr<SExpression> SExpression::LambdaCreate(LexicalScope* scope, SExpression* expr) {
    std::unique_ptr<SExpression> lambda (new SExpression((std::shared_ptr<LexicalScope> (new LexicalScope(scope)))));
    lambda.get() -> car_ = expr -> moveCar();
    lambda.get() -> cdr_ = expr -> moveCdr();
    if ((lambda.get() -> Car() == nullptr) || (lambda.get() -> Cdr() == nullptr) || lambda.get() -> Cdr() -> Cdr() == nullptr) {
        throw std::runtime_error("Syntax error : Lambda");
    }
    return lambda; 
}

std::unique_ptr<SExpression> SExpression::LambdaEvaluate(SExpression* lambda, SExpression* parameter_list) {
    if (!(lambda -> closure_scope -> IsInitiated()) || parameter_list -> GetType() != SExpression::Type::EMPTY) {
        lambda -> closure_scope -> Initiate();

        SExpression* lambda_list_length = lambda -> Car();
        size_t list_size_lambda = 0;

        while (lambda_list_length != nullptr) {
            if (lambda_list_length -> GetType() == SExpression::Type::EMPTY) {
                break;
            }
            ++list_size_lambda;
            lambda_list_length = lambda_list_length -> Cdr();
        }

        lambda_list_length = parameter_list;
        size_t list_size_parameters = 0;

        while (lambda_list_length != nullptr) {
            if (lambda_list_length -> GetType() == SExpression::Type::EMPTY) {
                break;
            }
            ++list_size_parameters;
            lambda_list_length = lambda_list_length -> Cdr();
        }

        if (list_size_lambda != list_size_parameters) {
            throw std::runtime_error("runtime error");
        }

        lambda_list_length = lambda -> Car();
        SExpression* value_iterator = parameter_list;
        while (lambda_list_length != nullptr) {
            if (lambda_list_length -> GetType() == SExpression::Type::EMPTY) {
                break;
            }
            value_iterator -> Car() -> Evaluate(lambda -> closure_scope.get());
            if (lambda_list_length -> Car() -> GetType() == SYMBOL) {
                lambda -> closure_scope -> setVariable(lambda_list_length -> Car() -> SymbolicValue(), value_iterator -> moveCar());
            }
            value_iterator = value_iterator -> Cdr();
            lambda_list_length = lambda_list_length -> Cdr();
        }
    }

    SExpression* list_validity_check = lambda -> Cdr();
    while (list_validity_check != nullptr) {
        if (list_validity_check -> GetType() == SExpression::Type::EMPTY) {
            break;
        }
        list_validity_check = list_validity_check -> Cdr();
    }

    if (list_validity_check == nullptr) {
        throw std::runtime_error("runtime error");
    }

    SExpression* curr_level = lambda -> Cdr();
    
    if (curr_level -> GetType() == SExpression::Type::EMPTY) {
        return std::unique_ptr<SExpression> (new SExpression(true));
    } else {
        if (curr_level -> Car() == nullptr) {
            throw std::runtime_error("runtime error");
        } else {
            SExpression* summand = curr_level -> Car();
            summand -> Evaluate(lambda -> closure_scope.get());
            if (summand -> GetType() == NUMBER) {
            }
            if (summand -> GetType() == EMPTY) {
            }
        }
    } 

    while (curr_level -> Cdr() -> GetType() != SExpression::Type::EMPTY) {
        curr_level = curr_level -> Cdr();

        if (curr_level -> Car() == nullptr) {
            throw std::runtime_error("runtime error");
        } else {
            SExpression* summand = curr_level -> Car();
            summand -> Evaluate(lambda -> closure_scope.get());
            if (summand -> GetType() == NUMBER) {
            }
            if (summand -> GetType() == EMPTY) {
            }
        }
        
    }
    
    return curr_level -> moveCar();
}
