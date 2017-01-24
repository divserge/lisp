#include <limits>
#include "lispp_sexpression.h"

std::unique_ptr<SExpression> SExpression::Sum(LexicalScope* scope, SExpression* expr) {
    int64_t sum = 0;
        
    SExpression* curr_level = expr;
    
    while (curr_level != nullptr) {
        SExpression* summand = nullptr;
        if (curr_level -> Car() == nullptr) {
            if (curr_level -> GetType() != SExpression::Type::EMPTY) {
                throw std::runtime_error("runtime error");
            }
        } else {
            summand = curr_level -> Car();
            summand -> Evaluate(scope);
            if (summand -> GetType() == SExpression::Type::NUMBER) {
                sum += summand -> NumericValue();
            } else {
                throw std::runtime_error("runtime error");
            }       
        }
        curr_level = curr_level -> Cdr();
    }
    return std::unique_ptr<SExpression> (new SExpression(sum));
}

std::unique_ptr<SExpression> SExpression::Max(LexicalScope* scope, SExpression* expr) {
    int64_t max = std::numeric_limits<int64_t>::min();
        
    SExpression* curr_level = expr;
    
    while (curr_level != nullptr) {
        SExpression* summand = nullptr;
        if (curr_level -> Car() == nullptr) {
            if (curr_level -> GetType() != SExpression::Type::EMPTY || curr_level == expr) {
                throw std::runtime_error("runtime error");
            }
        } else {
            summand = curr_level -> Car();
            summand -> Evaluate(scope);
            if (summand -> GetType() == SExpression::Type::NUMBER) {
                if (summand -> NumericValue() >= max) {
                    max = summand -> NumericValue();
                }
            } else {
                throw std::runtime_error("runtime error");
            }       
        }
        curr_level = curr_level -> Cdr();
    }
    return std::unique_ptr<SExpression> (new SExpression(max));
}

std::unique_ptr<SExpression> SExpression::Min(LexicalScope* scope, SExpression* expr) {
    int64_t min = std::numeric_limits<int64_t>::max();
        
    SExpression* curr_level = expr;
    
    while (curr_level != nullptr) {
        SExpression* summand = nullptr;
        if (curr_level -> Car() == nullptr) {
            if (curr_level -> GetType() != SExpression::Type::EMPTY || curr_level == expr) {
                throw std::runtime_error("runtime error");
            }
        } else {
            summand = curr_level -> Car();
            summand -> Evaluate(scope);
            if (summand -> GetType() == SExpression::Type::NUMBER) {
                if (summand -> NumericValue() <= min) {
                    min = summand -> NumericValue();
                }
            } else {
                throw std::runtime_error("runtime error");
            }       
        }
        curr_level = curr_level -> Cdr();
    }
    return std::unique_ptr<SExpression> (new SExpression(min));
}

std::unique_ptr<SExpression> SExpression::Mult(LexicalScope* scope, SExpression* expr) {
    int64_t sum = 1;
        
    SExpression* curr_level = expr;
    
    while (curr_level != nullptr) {
        SExpression* summand = nullptr;
        if (curr_level -> Car() == nullptr) {
            if (curr_level -> GetType() != SExpression::Type::EMPTY) {
                throw std::runtime_error("runtime error");
            }
        } else {
            summand = curr_level -> Car();
            summand -> Evaluate(scope);
            if (summand -> GetType() == SExpression::Type::NUMBER) {
                sum *= summand -> NumericValue();
            } else {
                throw std::runtime_error("runtime error");
            }       
        }
        curr_level = curr_level -> Cdr();
    }
    return std::unique_ptr<SExpression> (new SExpression(sum));
}

std::unique_ptr<SExpression> SExpression::Abs(LexicalScope* scope, SExpression* expr) {
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
        throw std::runtime_error("Runtime error : abs not an argument list");
    }

    if (list_size != 1) {
        throw std::runtime_error("Runtime error : abs invalid number of arguments");
    }

    list_validity_check = expr -> Car();
    expr -> Car() -> Evaluate(scope);
    if (expr -> Car() -> GetType() != SExpression::Type::NUMBER) {
        throw std::runtime_error("Runtime error : abs argument does not evaluate to a number");
    } else {
        return std::unique_ptr<SExpression>(new SExpression(std::abs(expr -> Car() -> NumericValue())));
    }
}

std::unique_ptr<SExpression> SExpression::Dec(LexicalScope* scope, SExpression* expr) {
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
        throw std::runtime_error("Runtime error : '-' not an argument list");
    }

    if (list_size == 0) {
        throw std::runtime_error("Runtime error : '-' invalid number of arguments");
    }

    expr -> Car() -> Evaluate(scope);

    if (expr -> Car() -> GetType() != NUMBER) {
        throw std::runtime_error("Runtime error : '-' argument does not evaluate to a number");
    }

    if (list_size == 1) {   
        return std::unique_ptr<SExpression> (new SExpression(0 - expr->Car()->NumericValue()));
    }

    int64_t sum = expr -> Car() -> NumericValue();
        
    SExpression* curr_level = expr -> Cdr();
    
    while (curr_level != nullptr) {
        SExpression* summand = nullptr;
        if (curr_level -> Car() == nullptr) {
            if (curr_level -> GetType() != SExpression::Type::EMPTY) {
                throw (std::runtime_error("Runtime error: '-' not an argument list"));
            }
        } else {
            summand = curr_level -> Car();
            summand -> Evaluate(scope);
            if (summand -> GetType() == SExpression::Type::NUMBER) {
                sum -= summand -> NumericValue();
            } else {
                throw std::runtime_error("Runtime error : '-' argument does not evaluate to a number");
            }       
        }
        curr_level = curr_level -> Cdr();
    }
    return std::unique_ptr<SExpression> (new SExpression(sum));
}

std::unique_ptr<SExpression> SExpression::Frac(LexicalScope* scope, SExpression* expr) {
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
       throw std::runtime_error("runtime error");
    }

    if (list_size == 0) {
        throw std::runtime_error("runtime error");
    }

    expr -> Car() -> Evaluate(scope);

    if (expr -> Car() -> GetType() != NUMBER) {
        throw std::runtime_error("runtime error");
    }

    if (list_size == 1) {   
        return std::unique_ptr<SExpression> (new SExpression(1/(expr->Car()->NumericValue())));
    }

    int64_t sum = expr -> Car() -> NumericValue();
        
    SExpression* curr_level = expr -> Cdr();
    
    while (curr_level != nullptr) {
        SExpression* summand = nullptr;
        if (curr_level -> Car() == nullptr) {
            if (curr_level -> GetType() != SExpression::Type::EMPTY) {
                throw std::runtime_error("runtime error");
            }
        } else {
            summand = curr_level -> Car();
            summand -> Evaluate(scope);
            if (summand -> GetType() == SExpression::Type::NUMBER) {
                sum /= summand -> NumericValue();
            } else {
                throw std::runtime_error("runtime error");
            }       
        }
        curr_level = curr_level -> Cdr();
    }
    return std::unique_ptr<SExpression> (new SExpression(sum));
}

std::unique_ptr<SExpression> SExpression::Greater(LexicalScope* scope, SExpression* expr) {
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
        return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));
    } else {
        curr_level -> Car() -> Evaluate(scope);
    } 

    curr_level = curr_level -> Cdr();

    if (curr_level -> GetType() == SExpression::Type::EMPTY) {
        return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));
    }


    SExpression* prev_level = expr;

    while (curr_level -> GetType() != SExpression::Type::EMPTY) {
        
        SExpression* summand = nullptr;
        
        if (curr_level -> Car() == nullptr) {
           throw std::runtime_error("runtime error");
        } else {
            summand = curr_level -> Car();
            summand -> Evaluate(scope);
            if (summand -> GetType() != SExpression::Type::NUMBER) {
                throw std::runtime_error("runtime error");
            } else {
                if (summand -> NumericValue() < prev_level -> Car() -> NumericValue()) {
                } else {
                    return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, false));
                }
            }
        }
        curr_level = curr_level -> Cdr();
        prev_level = prev_level -> Cdr();
    }
    return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));  
}

std::unique_ptr<SExpression> SExpression::Less(LexicalScope* scope, SExpression* expr) {
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
        return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));
    } else {
        curr_level -> Car() -> Evaluate(scope);
    } 

    curr_level = curr_level -> Cdr();

    if (curr_level -> GetType() == SExpression::Type::EMPTY) {
        return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));
    }


    SExpression* prev_level = expr;

    while (curr_level -> GetType() != SExpression::Type::EMPTY) {
        
        SExpression* summand = nullptr;
        
        if (curr_level -> Car() == nullptr) {
            throw std::runtime_error("runtime error");
        } else {
            summand = curr_level -> Car();
            summand -> Evaluate(scope);
            if (summand -> GetType() != SExpression::Type::NUMBER) {
                throw std::runtime_error("runtime error");
            } else {
                if (summand -> NumericValue() > prev_level -> Car() -> NumericValue()) {
                } else {
                    return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, false));
                }
            }
        }
        curr_level = curr_level -> Cdr();
        prev_level = prev_level -> Cdr();
    }
    return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));  
}

std::unique_ptr<SExpression> SExpression::GreaterOrEqual(LexicalScope* scope, SExpression* expr) {
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
        return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));
    } else {
        curr_level -> Car() -> Evaluate(scope);
    } 

    curr_level = curr_level -> Cdr();

    if (curr_level -> GetType() == SExpression::Type::EMPTY) {
        return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));
    }

    SExpression* prev_level = expr;

    while (curr_level -> GetType() != SExpression::Type::EMPTY) {
        
        SExpression* summand = nullptr;
        
        if (curr_level -> Car() == nullptr) {
            throw std::runtime_error("runtime error");
        } else {
            summand = curr_level -> Car();
            summand -> Evaluate(scope);
            if (summand -> GetType() != SExpression::Type::NUMBER) {
                throw std::runtime_error("runtime error");
            } else {
                if (summand -> NumericValue() <= prev_level -> Car() -> NumericValue()) {
                } else {
                    return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, false));
                }
            }
        }
        curr_level = curr_level -> Cdr();
        prev_level = prev_level -> Cdr();
    }
    return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));  
}

std::unique_ptr<SExpression> SExpression::LessOrEqual(LexicalScope* scope, SExpression* expr) {
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
        return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));
    } else {
        curr_level -> Car() -> Evaluate(scope);
    } 

    curr_level = curr_level -> Cdr();

    if (curr_level -> GetType() == SExpression::Type::EMPTY) {
        return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));
    }


    SExpression* prev_level = expr;

    while (curr_level -> GetType() != SExpression::Type::EMPTY) {
        
        SExpression* summand = nullptr;
        
        if (curr_level -> Car() == nullptr) {
            throw std::runtime_error("runtime error");
        } else {
            summand = curr_level -> Car();
            summand -> Evaluate(scope);
            if (summand -> GetType() != SExpression::Type::NUMBER) {
                throw std::runtime_error("runtime error");
            } else {
                if (summand -> NumericValue() >= prev_level -> Car() -> NumericValue()) {
                } else {
                    return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, false));
                }
            }
        }
        curr_level = curr_level -> Cdr();
        prev_level = prev_level -> Cdr();
    }
    return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));  
}

std::unique_ptr<SExpression> SExpression::Equal(LexicalScope* scope, SExpression* expr) {
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
        return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));
    } else {
        curr_level -> Car() -> Evaluate(scope);
    } 

    curr_level = curr_level -> Cdr();

    if (curr_level -> GetType() == SExpression::Type::EMPTY) {
        return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));
    }

    SExpression* prev_level = expr;

    while (curr_level -> GetType() != SExpression::Type::EMPTY) {
        
        SExpression* summand = nullptr;
        
        if (curr_level -> Car() == nullptr) {
            throw std::runtime_error("runtime error");
        } else {
            summand = curr_level -> Car();
            summand -> Evaluate(scope);
            if (summand -> GetType() != SExpression::Type::NUMBER) {
                throw std::runtime_error("runtime error");
            } else {
                if (summand -> NumericValue() == prev_level -> Car() -> NumericValue()) {
                } else {
                    return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, false));
                }
            }
        }
        curr_level = curr_level -> Cdr();
        prev_level = prev_level -> Cdr();
    }
    return std::unique_ptr<SExpression> (new SExpression(SExpression::Type::BOOL, true));  
}
