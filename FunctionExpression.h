//
// Created by Elliot on 5/6/2019.
//
#include "SymTab.hpp"
#include "Expr.hpp"
#include "Statements.hpp"

#ifndef EXPRINTER_FUNCTIONEXPRESSION_H
#define EXPRINTER_FUNCTIONEXPRESSION_H


class FunctionNode: public ExprNode {

public:
    virtual void print();
    virtual TypeDescriptor evaluate(SymTab &symTab);



};


class FunctionStatement: public Statement{
public:
    virtual void print();
    virtual void evaluate(SymTab &symTab);
};


#endif //EXPRINTER_FUNCTIONEXPRESSION_H
