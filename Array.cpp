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


// add array in TypeDescriptor (DONE), Expr (DONE), and parser.

class Array: public ExprNode {
public:
//    Array(Token token);
    Array(Token token): ExprNode{token} {}
    void print() {
        token().print();
    }

    TypeDescriptor evaluate(SymTab &symTab) {
        if( ! symTab.isDefined(token().getName())) {
            std::cout << "Use of undefined variable, " << token().getName() << std::endl;
            exit(1);
        }
        return symTab.getValueFor(token().getName());
    }
};





