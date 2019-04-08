//
// Created by Gabriel Duarte on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"


void SymTab::setValueFor(std::string vName, int value) {
    IntegerDescriptor *desc = new IntegerDescriptor(TypeDescriptor::INTEGER);
    desc->value.intValue =  value;
    symTab[vName] = desc;
}

void SymTab::setValueFor(std::string vName, std::string v) {
    StringDescriptor *desc = new StringDescriptor(TypeDescriptor::STRING);
    desc->stringValue =  v;
    symTab[vName] = desc;
}

bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}

TypeDescriptor* SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    //std::cout << "SymTab::getValueFor: " << vName << " contains " << symTab.find(vName)->second << std::endl;

    return symTab.find(vName)->second;
}