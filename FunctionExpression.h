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
    FunctionNode();
    FunctionNode(std::string name , Arguments* args);
    virtual void print();
    virtual TypeDescriptor evaluate(SymTab &symTab);
    Arguments* getArgs();
    std::string getName();



private:
    std::string _functionName;
    Arguments* _args;
    Statements* _stmt;
    std::vector<std::string> V_Names;

};


class FunctionStatement: public Statement{
public:
    FunctionStatement();
    FunctionStatement(FunctionNode*);
    virtual void print();
    virtual void evaluate(SymTab &symTab);

private:
    Token _functionName;
    Arguments* _args;
    Statements* _stmt;
    std::vector<std::string> V_Names;
    FunctionNode* _funcNode;

};


#endif //EXPRINTER_FUNCTIONEXPRESSION_H
