//
// Created by Elliot on 5/9/2019.
//

#include "ReturnStatement.h"

void ReturnStatement::print() {

}

void ReturnStatement::evaluate(SymTab  &symTab) {

    TypeDescriptor RV = _ret_val->evaluate(symTab);

    symTab.setReturnVal(RV);


}

ReturnStatement::ReturnStatement(ExprNode *ret_val) {

    _ret_val = ret_val;

}
