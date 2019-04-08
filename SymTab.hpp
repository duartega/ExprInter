//
// Created by Gabriel Duarte on 2/5/19.
//

#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include <string>
#include <map>
#include "Tokenizer.hpp"

// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.


struct TypeDescriptor {

    // This enum can expand to include additional types.
    enum types {INTEGER, STRING, BOOL};

    // constructor in itself

    TypeDescriptor(types type): _type{type} {}


    // returns the type
    types &type() { return _type; }

    // desctructor
    virtual ~TypeDescriptor() {}

private:
    types _type;
};

struct IntegerDescriptor: public TypeDescriptor {
    IntegerDescriptor(types descType):  TypeDescriptor(descType) {}

    union {
        int intValue;
    } value;
};

struct StringDescriptor: public TypeDescriptor {
    StringDescriptor(types descType): TypeDescriptor(descType) {}
    std::string stringValue;
};

class SymTab {
public:
    void setValueFor(std::string vName, int value);
    void setValueFor(std::string vName, std::string v);
    bool isDefined(std::string vName);
    TypeDescriptor* getValueFor(std::string vName);

private:
    std::map<std::string, TypeDescriptor*> symTab;
};

#endif //EXPRINTER_SYMTAB_HPP
