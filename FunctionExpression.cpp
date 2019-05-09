//
// Created by Elliot on 5/6/2019.
//

#include "FunctionExpression.h"


Arguments* FunctionNode::Args() {
    return _args;
}
TypeDescriptor FunctionNode::evaluate(SymTab &symTab) {

    auto v = _args->args();
    for (unsigned i = 0; i < v->size(); i++) {
        ExprNode* e = v->at(i);
        TypeDescriptor res = e->evaluate(symTab);
    }
    std::cout << std::endl;
    return TypeDescriptor();

}

void FunctionNode::print() {
    std::cout << "def ";
    std::cout << _functionName.getName();
    _stmt->print();

}

void FunctionStatement::evaluate(SymTab &symTab) {
    //just call the FunctionNode evaluate and get rid of the return type
    FunctionNode (SymTab &symTab);
}

void FunctionStatement::print() {

}
