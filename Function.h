//
// Created by Elliot on 5/9/2019.
//

#include <vector>
#include <string>



#ifndef EXPRINTER_FUNCTION_H
#define EXPRINTER_FUNCTION_H

class Statements;

class Function {

public:
    Function();
    Function(std::vector<std::string> PNames, Statements* stmts );
    std::vector<std::string> getP_Names();
    Statements* get_stmts();

private:
    std::vector<std::string> P_Names;
    Statements* _stmts;

};


#endif //EXPRINTER_FUNCTION_H
