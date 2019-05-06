//
// Created by Gabri on 4/26/2019.
//

#ifndef EXPRINTER_ARRAY_H
#define EXPRINTER_ARRAY_H

#include "Expr.hpp"
#include "Arguments.cpp"
#include "TypeDescriptor.cpp"
#include "Token.hpp"
#include "SymTab.hpp"

class Array: public ExprNode {
public:
    Array(Token token);
    virtual ~Array();
    virtual void print();
    virtual TypeDescriptor evaluate(SymTab &symTab);
};



#endif //EXPRINTER_ARRAY_H
