//
// Created by Gabri on 4/26/2019.
//

#include <vector>
#include <iostream>
#include "Expr.hpp"
#include "Arguments.cpp"
#include "TypeDescriptor.cpp"
#include "Token.hpp"
#include "SymTab.hpp"


// add array in TypeDescriptor (DONE), Expr (DONE), parser, statements.

class Array: public ExprNode {
public:

    Array(Arguments * _args): ExprNode{Token()} {_arguments = _args;}

    TypeDescriptor evaluate(SymTab &symTab) {
        std::vector<TypeDescriptor> ret;
        if (_arguments) {
            for (unsigned i = 0; i < _arguments->args()->size(); i++) {
                TypeDescriptor e = _arguments->args()->at(i)->evaluate(symTab);
                ret.push_back(e);
            }
            return TypeDescriptor(ret);
        }
        else
            return TypeDescriptor(ret);
    }
    void print()
    {
        for (unsigned i = 0; i < _arguments->args()->size(); i++)
        {
            ExprNode* e = _arguments->args()->at(i);
            e->print();
            std::cout << " ";
        }
    }
private:
    Arguments *_arguments;
};