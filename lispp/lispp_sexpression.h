#pragma once

#include <memory>
#include <iostream>
#include <cstdio>
#include <vector>
#include <stdexcept>
#include <cmath>

class LexicalScope;

class SExpression {
public:

    enum Type {
        BOOL,
        NUMBER,
        SYMBOL,
        EXPRESSION,
        EMPTY,
        OPERATOR,
        LAMBDA,
    };

    enum ApplicationType {
        SUM,
        MULT,
        DEC,
        FRAC,
        
        ABS,
        MAX,
        MIN,
        LEQ,
        GEQ,
        INTEQUAL,
        INTGREATER,
        INTLESS,

        DEFINE,
        SET,

        IF,
        AND,
        OR,
        NOT,

        CAR,
        CDR,
        SETCAR,
        SETCDR,
        LISTREF,
        LISTTAIL,
        QUOTE,
        
        ISPAIR,
        ISLIST,
        ISBOOL,
        ISNUM,
        ISSYM,
        ISNULL,
        
        LAMBDACREATE,

        CONS,
        LIST, 
    };

    SExpression(SExpression* other);
    SExpression& operator=(SExpression* other);
    SExpression(std::shared_ptr<LexicalScope>&& scope) : closure_scope (scope), type_ (LAMBDA) {} 
    SExpression() {}
    SExpression(ApplicationType apply_type) : apply_type_ (apply_type), type_ (OPERATOR) {}  
    SExpression(bool empty) : type_(EMPTY) {}
    SExpression(std::unique_ptr<SExpression>&& car,
                std::unique_ptr<SExpression>&& cdr,
                SExpression::Type type): car_(std::move(car)),
                cdr_(std::move(cdr)),
                type_(type) {}
    SExpression(int64_t value) : car_ (nullptr),
                                 cdr_ (nullptr),
                                 type_(Type::NUMBER),
                                 numeric_value_(value) {}
    SExpression (SExpression::Type type, bool istrue) : type_(type) { 
        if (istrue) {
            symbolic_value_ = "#t";
        } else {
            symbolic_value_ = "#f";
        }
    }
    SExpression(std::string value) : car_ (nullptr),
                                     cdr_ (nullptr),
                                     type_(Type::SYMBOL),
                                     symbolic_value_(value) {
                                        if (symbolic_value_ == "#t" || symbolic_value_ == "#f") {
                                            type_= BOOL;
                                        }
                                     }
    int64_t NumericValue() {
        return numeric_value_;
    }
    std::string SymbolicValue() {
        return symbolic_value_;
    }
    bool BooleanValue() {
        if (type_ != BOOL) {
            return true;
        } else {
            return symbolic_value_ == "#t";
        }
    }
    bool isQuoted() {
        return quoted_;
    }
    Type GetType() {
        return type_; }
    ApplicationType GetApplicationType() {
        return apply_type_;
    }
    
    SExpression* Car() { return car_.get(); }
    SExpression* Cdr() { return cdr_.get(); }

    std::unique_ptr<SExpression> moveCar() { return std::move(car_); }
    std::unique_ptr<SExpression> moveCdr() { return std::move(cdr_); }

    void SetCar(std::unique_ptr<SExpression>&& expression) { car_ = std::move(expression); }
    void SetCdr(std::unique_ptr<SExpression>&& expression) { cdr_ = std::move(expression); }

    //THIS IS TO CHANGE
    void Evaluate(LexicalScope* scope);
    void EvaluateAsSymbol(LexicalScope* scope);
    void EvaluateAsInteger(LexicalScope* scope);
    void EnQuote();
    std::unique_ptr<SExpression> Apply(LexicalScope* scope, SExpression* expr);
    std::string Print(bool open);
private:
    std::string PrintNumber();
    std::string PrintBool();
    std::string PrintSymbol();  
    std::unique_ptr<SExpression> Sum(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Mult(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Dec(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Frac(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> And(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Or(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> If(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Define(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> GetCar(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> GetCdr(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> ListRef(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> ListTail(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Set(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> SetCar(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> SetCdr(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Quote(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> LambdaCreate(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> LambdaEvaluate(SExpression* lambda, SExpression* parameter_list);
    std::unique_ptr<SExpression> Greater(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Less(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Equal(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Abs(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> IsPair(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> IsList(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> IsNumber(LexicalScope* scope, SExpression* expr);   
    std::unique_ptr<SExpression> IsBool(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> IsSymbol(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> IsNull(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> LessOrEqual(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> GreaterOrEqual(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Max(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Min(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Cons(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> Not(LexicalScope* scope, SExpression* expr);
    std::unique_ptr<SExpression> List(LexicalScope* scope, SExpression* expr);

    bool quoted_ = false;
    int64_t numeric_value_;
    std::string symbolic_value_;
    std::unique_ptr<SExpression> car_ = nullptr;
    std::unique_ptr<SExpression> cdr_ = nullptr;
    Type type_ = EXPRESSION;
    ApplicationType apply_type_ = SUM;
    std::shared_ptr<LexicalScope> closure_scope = nullptr;
};
