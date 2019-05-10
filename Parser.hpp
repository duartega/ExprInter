
#ifndef __PARSER_HPP
#define __PARSER_HPP

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "Expr.hpp"
#include "Statements.hpp"
#include "Array.cpp"
#include "Function.h"
#include "FunctionExpression.h"

#include<vector>
#include<iostream>
#include<map>


class Parser {
public:
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

    Statements *statements();

    AssignmentStatement *assignStatement(Token VarName);
    PrintStatement *printStatement();
    ForStatement* forStatement();
    IfElseStatement* ifElseStatement();
    FunctionDef* functionDef();
    FunctionNode* functionNode(Token VarName);

    ExprNode *rel_expr();
    ExprNode *rel_term();
    ExprNode *rel_primary();

    ExprNode *arith_expr();
    ExprNode *arith_term();
    ExprNode *arith_primary();

    Arguments* arguments();

    std::string id();

private:
    Tokenizer &tokenizer;

    void die(std::string where, std::string message, Token &token);

};

#endif

