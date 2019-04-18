//
// Created by Gabriel Duarte on 3/25/19.
//

#include "Statements.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include <map>

Statement::Statement() {}

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }


AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}
Print::Print() : _lhsVariable{""}, _rhsExpression{nullptr} {}
For::For() : _forAssign{nullptr}, _forComp{nullptr}, _forIncrement{nullptr}, _forBodyStatement{nullptr} {}



AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}
Print::Print(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}
For::For(AssignmentStatement *assign1, ExprNode *expr1, AssignmentStatement *assign2, Statements *state):
        _forAssign{assign1}, _forComp{expr1}, _forIncrement{assign2}, _forBodyStatement{state} {}


std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}
std::string &Print::lhsVariable() {
    return _lhsVariable;
}


ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

ExprNode *&Print::rhsExpression() {
    return _rhsExpression;
}


AssignmentStatement *&For::forAssign() {
    return _forAssign;
}

ExprNode *&For::forComp() {
    return _forComp;
}

AssignmentStatement *&For::forIncrement() {
    return _forIncrement;
}

Statements *&For::forBodyStatement() {
    return _forBodyStatement;
}

void For::print() {
    std::cout << "for i in range ("; _forAssign->print();
    _forComp->print(); _forIncrement->print();
    std::cout << "):"; _forBodyStatement->print();
}

void Statements::print() {
    for (auto s: _statements)
        s->print();
}
void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    std::cout << std::endl;
}
void Print::print() { // This is a debug print
    std::cout << _lhsVariable << " ";
    _rhsExpression->print();
    std::cout << std::endl;
}

// This is just for our debugging, this is not for actually printing the values
// This is left for evaluate
//void For::print() {
//    std::cout << "for (";
//    _assign1->print(); std::cout << "; ";
//    _condition->print(); std::cout << "; ";
//    _assign2->print();
//    std::cout << ")";
//    std::cout << "{";
//    std::cout << std::endl;
//    _additional_statement->print();
//    std::cout << "}";
//    std::cout << std::endl;
//}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements) {
        s->evaluate(symTab);
        std::cout << std::endl;
    }
}
void For::evaluate(SymTab &symTab) {
    int a1 = forAssign()->rhsExpression()->evaluate(symTab);
    symTab.setValueFor(forAssign()->lhsVariable(), a1);

    int condit = forComp()->evaluate(symTab);

    // Still need to fix this
    while (condit == 1) {
        int a2 = forIncrement()->rhsExpression()->evaluate(symTab);
        symTab.setValueFor(forIncrement()->lhsVariable(), a2);
        condit = forComp()->evaluate(symTab);
        forBodyStatement()->evaluate(symTab);
    }
}
void AssignmentStatement::evaluate(SymTab &symTab) {

    // We are checking to see what kind of assignment we want to do
    bool rhsInt = rhsExpression()->token().isWholeNumber();
    bool rhsString = rhsExpression()->token().isString();

    if (rhsInt) { // Assign an int
        int rhs = rhsExpression()->evaluate(symTab);
        symTab.setValueFor(lhsVariable(), rhs);
    } else if (rhsString) { // Assign a string
        std::string rhs = rhsExpression()->token().getString();
        symTab.setValueFor(lhsVariable(), rhs);
    } else if (rhsExpression()->evaluate(symTab)) { // So we can add inline
        symTab.setValueFor(lhsVariable(), rhsExpression()->evaluate(symTab));
    } else if (rhsExpression()->token().isArithmeticOperator()) // So we can add inline
        symTab.setValueFor(lhsVariable(), rhsExpression()->evaluateString(symTab));

}
void Print::evaluate(SymTab &symTab) {
    std::cout << "----------\n";
    std::cout << "|PRINTING|\n";
    std::cout << "----------\n";

    if( rhsExpression()->token().isString()) { // print "a ="
        rhsExpression()->print(); // because we didn't store "a ="
    }  else  if(rhsExpression()->token().isComma()) // prints test list
        rhsExpression()->evaluate(symTab);
    else {
        // Get value of a single print variable
        auto valOfToken = symTab.getValueFor(rhsExpression()->token().getName());
        StringDescriptor *stringCast = dynamic_cast<StringDescriptor *> (valOfToken);
        if (stringCast != nullptr)
            std::cout << stringCast->stringValue;
        IntegerDescriptor *integerCast = dynamic_cast<IntegerDescriptor *> (valOfToken);
        if (integerCast != nullptr)
            std::cout << integerCast->value.intValue;
        return;
    }


}
