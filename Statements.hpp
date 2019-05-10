//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "Expr.hpp"
#include "SymTab.hpp"
#include "Arguments.cpp"
#include "Range.cpp"
#include "Function.h"

// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

class Statement {
public:
    Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;
};


// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);
    void print();

private:
    std::vector<Statement *> _statements;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);

    std::string &lhsVariable();
    ExprNode *&rhsExpression();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
};

class PrintStatement: public Statement
{
public:
    PrintStatement();
    PrintStatement(Arguments*);

    Arguments* Items();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    Arguments* _items;
};

class FunctionDef: public Statement
{
public:
    FunctionDef();
    FunctionDef(std::string funcName, std::vector<std::string> args, Statements* stmts);

    std::string &functionName();
    std::vector<std::string> Args();


    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _functionName;
    std::vector<std::string> _args;
    Statements* _stmt;

};

class ForStatement: public Statement
{
public:
    ForStatement();
    ForStatement(Variable* var, Range* r, Statements* stmts);

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    ExprNode* _var;
    Range* _range;
    Statements* _stmts;
};


class IfStatement: public Statement
{
public:
    IfStatement();
    IfStatement(ExprNode* condition, Statements* statements);

    virtual void evaluate(SymTab& symTab);
    virtual void print();
    bool got_evaluated() { return got_evald; };

private:
    ExprNode* _cond;
    Statements* _stmts;
    bool got_evald;

};

class IfElseStatement: public Statement
{
public:
    IfElseStatement();
    IfElseStatement(std::vector<IfStatement*>* ifs, Statements* p_else);

    virtual void evaluate(SymTab& symTab);
    virtual void print();

private:
    std::vector<IfStatement*>* _ifs;
    Statements* _else;
};

#endif //EXPRINTER_STATEMENTS_HPP
