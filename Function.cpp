//
// Created by Elliot on 5/9/2019.
//

#include "Function.h"

Statements *Function::get_stmts() {

    return _stmts;
}

std::vector<std::string> Function::getP_Names() {

    return P_Names;
}

Function::Function() {


}

Function::Function(std::vector<std::string> PNames, Statements *stmts) {

    P_Names = PNames;

    _stmts = stmts;
}
