//
// Created by Elliot on 5/6/2019.
//

#include "FunctionExpression.h"



TypeDescriptor FunctionNode::evaluate(SymTab &symTab) {

    Function* function =  symTab.getFunction(_functionName);

    _stmt =  function->get_stmts();
    V_Names = function->getP_Names();

    auto v = _args->args();
    std::vector<TypeDescriptor> TypeVector;
    for (unsigned i = 0; i < v->size(); i++) {
        ExprNode* e = v->at(i);
        TypeDescriptor res = e->evaluate(symTab);
        TypeVector.push_back(res);

    }
    symTab.openScope();
    for(auto i = 0; i < TypeVector.size() ;i++)
    {
        symTab.setValueFor(V_Names[i],TypeVector[i]);
    }
    _stmt->evaluate(symTab);

    symTab.closeScope();

    return TypeDescriptor();

}

void FunctionNode::print() {
    std::cout << "def ";
    std::cout << _functionName;
    _stmt->print();

}

FunctionNode::FunctionNode(): ExprNode(Token()) {

}

FunctionNode::FunctionNode(std::string name, Arguments *args): ExprNode(Token()) {

    _args = args;
    _functionName = name;

}

std::string FunctionNode::getName() {
    return _functionName;
}

Arguments *FunctionNode::getArgs() {
    return _args;
}

void FunctionStatement::evaluate(SymTab &symTab) {
    //just call the FunctionNode evaluate and get rid of the return type

    if(!_funcNode) {

        std::cout<< "death";
    }
    _funcNode->evaluate(symTab);
}

void FunctionStatement::print() {


}

FunctionStatement::FunctionStatement() {




}

FunctionStatement::FunctionStatement(FunctionNode* functionNode){

    _funcNode = functionNode;

}
