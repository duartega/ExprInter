//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"

// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements) {
        s->print();
        std::cout << std::endl;
    }
}

void Statements::evaluate(SymTab &symTab)
{
    for (auto s: _statements)
    {
        s->evaluate(symTab);
        if(s->isReturnStatment())
        {
            break;

        }
    }
}

/*
 * Assignment Statement
 */

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}
AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr): _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    TypeDescriptor rhs = rhsExpression()->evaluate(symTab);
    symTab.setValueFor(lhsVariable(), rhs);
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
}

/*
 * Function Definition
 */

FunctionDef::FunctionDef() : _functionName{}, _args{nullptr}, _stmt{nullptr} {}
FunctionDef::FunctionDef(std::string funcName, std::vector<std::string> args, Statements* stmts) : _functionName{funcName}, _args{args}, _stmt{stmts} {}


std::vector<std::string> FunctionDef::Args() {
    return _args;
}

void FunctionDef::evaluate(SymTab &symTab)
{

    auto func = new Function(_args, _stmt);
    symTab.setFunction( _functionName ,func);

}

void FunctionDef::print() {
    std::cout << "def ";
    std::cout << _functionName;
    _stmt->print();
}

/*
 * Print Statement
 */

PrintStatement::PrintStatement() : _items{nullptr} {}
PrintStatement::PrintStatement(Arguments* v) : _items{v} {}

Arguments* PrintStatement::Items() {
    return _items;
}

void PrintStatement::evaluate(SymTab &symTab) {
    auto v = _items->args();
    for (unsigned i = 0; i < v->size(); i++) {
        ExprNode* e = v->at(i);
        TypeDescriptor res = e->evaluate(symTab);
        res.print();
        std::cout << " ";
    }
    std::cout << std::endl;
}

void PrintStatement::print() {
    std::cout << "print ";
    _items->print();
}

/*
 * For Statement
 */

ForStatement::ForStatement(): _var{nullptr}, _range{nullptr}, _stmts{nullptr} {}
ForStatement::ForStatement(Variable* var, Range* r, Statements* stmts): _var{var}, _range{r}, _stmts{stmts} {}

void ForStatement::evaluate(SymTab &symTab) {
    int i;

    _range->begin(symTab);
    while (!_range->done()) {
        i = _range->next();
        symTab.setValueFor(_var->token().getName(), i);
        _stmts->evaluate(symTab);
    }
}

void ForStatement::print() {
    std::cout << "for ";
    _var->print();
    std::cout << " in ";
    _range->print();
    std::cout << std::endl;
    std::cout << "-----\n";
    _stmts->print();
    std::cout << "-----";
}


IfStatement::IfStatement(): _cond{nullptr}, _stmts{nullptr}, got_evald{false} {}
IfStatement::IfStatement(ExprNode *condition, Statements *statements): _cond{condition}, _stmts{statements}, got_evald{false} {}

void IfStatement::evaluate(SymTab &symTab) {
    TypeDescriptor res = _cond->evaluate(symTab);
    if (res.get_type() != TypeDescriptor::BOOLEAN) {
        std::cout << "Non boolean value in if statement: ";
        res.print();
        std::cout << std::endl;
        exit(4);
    }
    if (res.get_bvalue()) {
        _stmts->evaluate(symTab);
        got_evald = true;
    } else {
        got_evald = false;
    }
}

void IfStatement::print() {
    std::cout << "if ";
    _cond->print();
    std::cout << ":\n-----\n";
    _stmts->print();
    std::cout << "-----";
}

IfElseStatement::IfElseStatement(): _ifs{nullptr}, _else{nullptr} {}
IfElseStatement::IfElseStatement(std::vector<IfStatement *> *ifs, Statements *p_else): _ifs{ifs}, _else{p_else} {}

void IfElseStatement::evaluate(SymTab &symTab) {
    bool ignore_else = false;
    for (unsigned i = 0; i < _ifs->size(); i++) {
        auto p = _ifs->at(i);
        p->evaluate(symTab);
        if (p->got_evaluated()) {
            ignore_else = true;
            break;
        }
    }
    if (!ignore_else) {
        if (_else) {
            _else->evaluate(symTab);
        }
    }
}

void IfElseStatement::print() {
    for (unsigned i = 0; i < _ifs->size(); i++) {
        auto p = _ifs->at(i);
        if (i != 0) {
            std::cout << "el";
        }
        p->print();
    }
    if (_else) {
        std::cout << "\nelse: \n-----\n";
        _else->print();
        std::cout << "-----";
    }
}