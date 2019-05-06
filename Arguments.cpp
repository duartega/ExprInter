//
// Created by George on 3/29/2019.
//
#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <vector>
#include <iostream>
#include "Expr.hpp"

class Arguments
{
public:

    Arguments(): _args{nullptr} {}
    Arguments(std::vector<ExprNode*>* v): _args{v} {}

    std::vector<ExprNode*>* args() { return _args; }
    int count() {return _args->size();}

    void print()
    {
        for (unsigned i = 0; i < _args->size(); i++)
        {
            ExprNode* e = _args->at(i);
            e->print();
            std::cout << " ";
        }
    }

private:
    std::vector<ExprNode*>* _args;
};

#endif //ARGUMENTS_H
