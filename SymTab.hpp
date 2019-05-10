//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include <string>
#include <map>
#include <stack>
#include "TypeDescriptor.cpp"

// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.

class Function;

class SymTab {
public:
    void setValueFor(std::string vName, int value);
    void setValueFor(std::string vName, std::string value);
    void setValueFor(std::string vName, bool value);
    void setValueFor(std::string vName, TypeDescriptor value);

    bool isDefined(std::string vName);

    int getIntValueFor(std::string vName);
    std::string getStringValueFor(std::string vName);
    bool getBoolValueFor(std::string vName);
    TypeDescriptor getValueFor(std::string vName);

    void openScope();
    void closeScope();

    Function* getFunction(std::string name);
    void setFunction(std::string name, Function* func );

    void setReturnVal(TypeDescriptor value);
    TypeDescriptor getReturnValue();
    bool hasReturnValue();

    TypeDescriptor getReturnValue(std::string vName) { return _returnValue; };

private:

    std::vector<std::map<std::string, TypeDescriptor> >scope;
    std::map<std::string, Function*> functionTab;
    //std::map<std::string, TypeDescriptor> globalSymTab;
    TypeDescriptor _returnValue;
    bool hasReturnVal = false;
};

#endif //EXPRINTER_SYMTAB_HPP
