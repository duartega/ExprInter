//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include<iostream>
#include "Expr.hpp"

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

TypeDescriptor InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    TypeDescriptor lValue_desc = left()->evaluate(symTab);
    TypeDescriptor rValue_desc = right()->evaluate(symTab);

    if (lValue_desc.get_type() == TypeDescriptor::INTEGER && rValue_desc.get_type() == TypeDescriptor::INTEGER) {
        int lValue = lValue_desc.get_ivalue();
        int rValue = rValue_desc.get_ivalue();

        if (token().isAdditionOperator())
            return TypeDescriptor(lValue + rValue);
        else if (token().isSubtractionOperator())
            return TypeDescriptor(lValue - rValue);
        else if (token().isMultiplicationOperator())
            return TypeDescriptor(lValue * rValue);
        else if (token().isDivisionOperator())
            return TypeDescriptor(lValue / rValue); // division by zero?
        else if (token().isModuloOperator())
            return TypeDescriptor(lValue % rValue);
        else if (token().isGreaterThanOperator())
            return TypeDescriptor((bool) (lValue > rValue));
        else if (token().isGreaterThanEqualOperator())
            return TypeDescriptor((bool) (lValue >= rValue));
        else if (token().isLessThanOperator())
            return TypeDescriptor((bool) (lValue < rValue));
        else if (token().isLessThanEqualOperator())
            return TypeDescriptor((bool) (lValue <= rValue));
        else if (token().isComparisonOperator())
            return TypeDescriptor((bool) (lValue == rValue));
        else if (token().isExclamComparisonOperator())
            return TypeDescriptor((bool) (lValue != rValue));
        else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator for int on int\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    } else if (lValue_desc.get_type() == TypeDescriptor::BOOLEAN && rValue_desc.get_type() == TypeDescriptor::BOOLEAN) {
        bool lValue = lValue_desc.get_bvalue();
        bool rValue = rValue_desc.get_bvalue();

        if (token().isGreaterThanOperator())
            return TypeDescriptor(lValue > rValue);
        else if (token().isGreaterThanEqualOperator())
            return TypeDescriptor(lValue >= rValue);
        else if (token().isLessThanOperator())
            return TypeDescriptor(lValue < rValue);
        else if (token().isLessThanEqualOperator())
            return TypeDescriptor(lValue <= rValue);
        else if (token().isComparisonOperator())
            return TypeDescriptor(lValue == rValue);
        else if (token().isExclamComparisonOperator())
            return TypeDescriptor(lValue != rValue);
        else if (token().isAnd())
            return lValue && rValue;
        else if (token().isOr())
            return lValue || rValue;
        else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator for bool on bool\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    } else if (lValue_desc.get_type() == TypeDescriptor::STRING && rValue_desc.get_type() == TypeDescriptor::STRING) {
        std::string lValue = lValue_desc.get_svalue();
        std::string rValue = rValue_desc.get_svalue();

        if (token().isAdditionOperator())
            return TypeDescriptor(lValue + rValue);
        else if (token().isGreaterThanOperator())
            return TypeDescriptor((bool) (lValue > rValue));
        else if (token().isGreaterThanEqualOperator())
            return TypeDescriptor((bool) (lValue >= rValue));
        else if (token().isLessThanOperator())
            return TypeDescriptor((bool) (lValue < rValue));
        else if (token().isLessThanEqualOperator())
            return TypeDescriptor((bool) (lValue <= rValue));
        else if (token().isComparisonOperator())
            return TypeDescriptor((bool) (lValue == rValue));
        else if (token().isExclamComparisonOperator())
            return TypeDescriptor((bool) (lValue != rValue));
        else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator for string on string\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    }

    if (lValue_desc.get_type() == TypeDescriptor::INTEGER && rValue_desc.get_type() == TypeDescriptor::STRING) {
        int i = lValue_desc.get_ivalue();
        std::string lValue = std::to_string(i);
        std::string rValue = rValue_desc.get_svalue();
        if (token().isAdditionOperator()) {
            return lValue + rValue;
        } else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator for int on string\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    }

    if (lValue_desc.get_type() == TypeDescriptor::STRING && rValue_desc.get_type() == TypeDescriptor::INTEGER) {
        std::string lValue = lValue_desc.get_svalue();
        int i = rValue_desc.get_ivalue();
        std::string rValue = std::to_string(i);
        if (token().isAdditionOperator()) {
            return lValue + rValue;
        } else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator for string on int\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    } else {
        std::cout << "Type mismatch in InfixExprNode::evaluate" << std::endl;
        lValue_desc.print();
        std::cout<<":";
        rValue_desc.print();
        exit(2);
    }
}

void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}

// WHoleNumber
WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

TypeDescriptor WholeNumber::evaluate(SymTab &symTab) {
    //std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    return TypeDescriptor(token().getWholeNumber());
}

// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

TypeDescriptor Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    //std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    return symTab.getValueFor(token().getName());
}

InfixExprNode::~InfixExprNode() {
    delete _left;
    delete _right;
}

StringValue::StringValue(Token token): ExprNode(token) {}
void StringValue::print() {
    token().print();
}

TypeDescriptor StringValue::evaluate(SymTab &symTab) {
    return TypeDescriptor(token().getName());
}

BooleanValue::BooleanValue(Token token): ExprNode(token) {}

void BooleanValue::print() {
    token().print();
}

TypeDescriptor BooleanValue::evaluate(SymTab &symTab)
{
    if (token().getName() == "True") {
        return TypeDescriptor(true);
    } else if (token().getName() == "False") {
        return TypeDescriptor(false);
    } else {
        std::cout << "Boolean got set to something other than true or false. That's yo bad..." << std::endl;
        exit(5);
    }
}

ExprNode::~ExprNode() = default;
WholeNumber::~WholeNumber() = default;
StringValue::~StringValue() = default;
BooleanValue::~BooleanValue() = default;
Variable::~Variable() = default;