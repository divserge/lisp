#include <memory>
#include <iostream>
#include <cstdio>
#include <vector>
#include <stdexcept>
#include <cmath>

#include "lispp_sexpression.h"
#include "lispp_lexical_scope.h"

SExpression::SExpression(SExpression* other) :  type_(other -> type_),
                                   apply_type_(other -> apply_type_),
                                   numeric_value_(other -> numeric_value_),
                                   symbolic_value_(other -> symbolic_value_),
                                   quoted_(other -> quoted_),
                                   closure_scope(other -> closure_scope)
                                   {
    if (other -> car_ != nullptr) {
        car_ = std::unique_ptr<SExpression>(new SExpression(other -> Car()));
    }
    if (other -> cdr_ != nullptr) {
        cdr_ = std::unique_ptr<SExpression>(new SExpression(other -> Cdr()));
    }
}

SExpression& SExpression::operator=(SExpression* other) {
    if (other != nullptr) {
    type_ = (other->type_);
    apply_type_ = (other->apply_type_);
    numeric_value_ = (other->numeric_value_);
    symbolic_value_ = (other->symbolic_value_);
    quoted_ = (other->quoted_);
    car_ = (std::move(other->car_)); 
    cdr_ = (std::move(other->cdr_));
    closure_scope = other -> closure_scope;
    }
    return *this;
}

void SExpression::EnQuote() {
    quoted_ = true;
    if (car_ != nullptr) {
        Car() -> EnQuote();
    }
    if (cdr_ != nullptr) {
        Cdr() -> EnQuote();
    }
}

std::string SExpression::Print(bool open) {
    if (type_ == NUMBER) {
        return std::to_string(numeric_value_);
    }

    if (type_ == BOOL || type_ == SYMBOL) {
        return symbolic_value_;
    }

    std::string compound_string;
    
    if (open) {
        compound_string += '(';
    }

    if (type_ == EXPRESSION) {

        if (Car() -> GetType() == EXPRESSION) {
            compound_string += Car() -> Print(true);
        } else {
            compound_string += Car() -> Print(false);
        }
        if (Cdr () == nullptr) {
            compound_string += ')';
        } else if (Cdr() -> GetType() == EMPTY) {
            compound_string += ')';
        } else if (Cdr() -> GetType() != EXPRESSION) {
            compound_string += " . " + Cdr() -> Print(false) + ')';  
        } else {
            compound_string += " " + Cdr() -> Print(false);
        }
    }

    if (type_ == EMPTY) {
        compound_string += ')';
    }

    return compound_string;
}

std::unique_ptr<SExpression> SExpression::Apply(LexicalScope* scope, SExpression* expr) {
    switch (apply_type_) {
        case SUM:
            return Sum(scope, expr);
        case MULT:
            return Mult(scope, expr);
        case OR:
            return Or(scope, expr);
        case AND:
            return And(scope, expr);
        case IF:
            return If(scope, expr);
        case DEFINE:
            return Define(scope, expr);
        case LISTREF:
            return ListRef(scope, expr);
        case LISTTAIL:
            return ListTail(scope, expr);
        case QUOTE:
            return Quote(scope, expr);
        case CAR:
            return GetCar(scope, expr);
        case CDR:
            return GetCdr(scope, expr);
        case SETCAR:
            return SetCar(scope, expr);
        case SETCDR:
            return SetCdr(scope, expr);
        case LAMBDACREATE:
            return LambdaCreate(scope, expr);
        case SET:
            return Set(scope, expr);
        case INTEQUAL:
            return Equal(scope, expr);
        case INTGREATER:
            return Greater(scope, expr);
        case INTLESS:
            return Less(scope, expr);
        case DEC:
            return Dec(scope, expr);
        case FRAC:
            return Frac(scope, expr);
        case ISLIST:
            return IsList(scope, expr);
        case ISBOOL:
            return IsBool(scope, expr);
        case ISSYM:
            return IsSymbol(scope, expr);
        case ISNUM:
            return IsNumber(scope, expr);
        case ISNULL:
            return IsNull(scope, expr);
        case ABS:
            return Abs(scope, expr);
        case ISPAIR:
            return IsPair(scope, expr);
        case LEQ:
            return LessOrEqual(scope, expr);
        case GEQ:
            return GreaterOrEqual(scope, expr);
        case MAX:
            return Max(scope, expr);
        case MIN:
            return Min(scope, expr);
        case CONS:
            return Cons(scope, expr);
        case LIST:
            return List(scope, expr);
        case NOT:
            return Not(scope, expr);
        default:
            return nullptr;
    }
}

std::unique_ptr<SExpression> SExpression::Quote(LexicalScope* scope, SExpression* expr) {
    SExpression* list_validity_check = expr;
    expr -> Car() -> EnQuote();
    return expr -> moveCar();
}

void SExpression::Evaluate(LexicalScope* scope) {    
    if (quoted_) {
        quoted_ = false;
        if (Car()) {
            Car() -> Evaluate(scope);
        }
        if (Cdr()) {
            Cdr() -> Evaluate(scope);
        }
        return;
    }
    if (type_ == NUMBER) {
        return;
    }
    if (type_ == BOOL) {
        return;
    }
    if (type_ == EMPTY) {
        throw std::runtime_error("Runtime error : empty list is not self-evaluating");
        return;
    }
    if (type_ == EXPRESSION) {
        Car() -> Evaluate(scope);
        if (Car() -> GetType() == LAMBDA) {
            std::unique_ptr<SExpression> expression = Car() -> LambdaEvaluate(Car(), Cdr());
            if (expression == nullptr) {
                type_ = SYMBOL;
                throw std::runtime_error("runtime error");
                quoted_ = true;
                return;
            }
            *this = expression.get();
            return;
        }
        if (Car() -> GetType() != OPERATOR) {
            throw std::runtime_error("Runtime error : Can not be applied");
        } else {
            std::unique_ptr<SExpression> expression = Car() -> Apply(scope, Cdr());
            if (expression == nullptr) {
                type_ = SYMBOL;
                symbolic_value_ = "Unspecified output";
                quoted_ = true;
                return;
            }

            *this = expression.get();
            return;
        }
    }
    if (type_ == SYMBOL) {
        int64_t numeric_value;
        std::string message = " ";
        try {
            numeric_value = stoi(SymbolicValue());
        } catch (const std::invalid_argument& ia) {
            message = ia.what();
        }
        if (message == " ") {
            type_ = NUMBER;
            numeric_value_ = numeric_value;
            return;
        }
        SExpression* reference = scope -> LookUp(SymbolicValue());
        if (reference == nullptr) {
            throw std::runtime_error("Undefined variable");
        } else {
            bool working_with_quoted = false;
            SExpression copy_expression;
            if (reference -> Car() != nullptr) {
                if (reference -> Car() -> SymbolicValue() == "quote") {
                    working_with_quoted = true;
                }
            }
            if (!working_with_quoted) {
                reference -> Evaluate(scope);
            }
            copy_expression.type_ = reference -> GetType();
            copy_expression.apply_type_ = reference -> GetApplicationType();
            copy_expression.numeric_value_ = reference -> NumericValue();
            copy_expression.symbolic_value_ = reference -> SymbolicValue();
            copy_expression.quoted_ = reference -> isQuoted();
            copy_expression.closure_scope = reference -> closure_scope;

            if (reference -> Car() != nullptr) {
                copy_expression.car_ = std::unique_ptr<SExpression> (new SExpression(reference -> Car()));
            } else {
                copy_expression.car_ = nullptr;
            }

            if (reference -> Cdr() != nullptr) {
                copy_expression.cdr_ = std::unique_ptr<SExpression> (new SExpression(reference -> Cdr()));
            } else {
                copy_expression.cdr_ = nullptr;
            };

            if (working_with_quoted) {
                copy_expression.Evaluate(scope);
            }

            type_ = copy_expression.GetType();
            apply_type_ = copy_expression.GetApplicationType();
            numeric_value_ = copy_expression.NumericValue();
            symbolic_value_ = copy_expression.SymbolicValue();
            quoted_ = copy_expression.isQuoted();
            closure_scope = copy_expression.closure_scope;

            if (copy_expression.Car() != nullptr) {
                car_ = std::unique_ptr<SExpression> (new SExpression(copy_expression.Car()));
            } else {
                cdr_ = nullptr;
            }

            if (copy_expression.Cdr() != nullptr) {
                cdr_ = std::unique_ptr<SExpression> (new SExpression(copy_expression.Cdr()));
            } else {
                cdr_ = nullptr;
            }

            Evaluate(scope);
            
            return;
        }
    }
}
