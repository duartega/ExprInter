//
// Created by Elliot on 5/9/2019.
//
#include "Statements.hpp"
#ifndef EXPRINTER_RETURNSTATEMENT_H
#define EXPRINTER_RETURNSTATEMENT_H


class ReturnStatement : public Statement {

public:

    ReturnStatement(ExprNode* ret_val);
    virtual void evaluate();
    virtual void print();


private:
    ExprNode* _ret_val;





};


#endif //EXPRINTER_RETURNSTATEMENT_H