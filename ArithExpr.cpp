//
// Created by Gabriel Duarte on 2/5/19.
//

#include<iostream>
#include <cstring>
#include "ArithExpr.hpp"
#include "SymTab.hpp"

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

std::string InfixExprNode::evaluateString(SymTab &symTab) {
    auto lValue = left()->token().getString().c_str();
    auto rValue = right()->token().getString().c_str();
    if( token().isAdditionOperator() )
        return lValue;
}

int InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    auto lValue = left()->evaluate(symTab);
    auto rValue = right()->evaluate(symTab);
//    std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
    if( token().isAdditionOperator() )
        return lValue + rValue;
     else if(token().isSubtractionOperator())
        return lValue - rValue;
    else if(token().isMultiplicationOperator())
        return lValue * rValue;
    else if(token().isDivisionOperator())
        return lValue / rValue; // division by zero?
    else if( token().isModuloOperator() )
        return lValue % rValue;
    else if( token().isGt())
        return lValue > rValue;
    else if( token().isGte())
        return lValue >= rValue;
    else if( token().isLt()){
//        std::cout << "EVALUATING: " << lValue << " < " << rValue << std::endl;
        return lValue < rValue;
    }
    else if( token().isLte())
        return lValue <= rValue;
    else if( token().isEqualOperator())
        return lValue == rValue;
    else if( token().isNotEqualOperator())
        return lValue != rValue;
        else if ( token().isComma() ){
        std::cout << "RVAL: " << rValue << '\n';
        return rValue;
    }
    else {
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
}

void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}

WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

int WholeNumber::evaluate(SymTab &symTab) {
    return token().getWholeNumber();
}

std::string WholeNumber::evaluateString(SymTab &symTab) {
    return token().getString();
}

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

std::string Variable::evaluateString(SymTab &symTab) {
    StringDescriptor *desc = dynamic_cast<StringDescriptor*> (symTab.getValueFor(token().getName()));
    return desc->stringValue;
}

int Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable from Variable::evaluate, " << token().getName() << std::endl;
        exit(1);
    }
//    std::cout << "Token Name in Variable Eval: " << token().getName() << std::endl;
    IntegerDescriptor *desc = dynamic_cast<IntegerDescriptor*> (symTab.getValueFor(token().getName()));
    std::cout << desc->value.intValue << '\n';
    return desc->value.intValue;
}


