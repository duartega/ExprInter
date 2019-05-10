//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include <stack>
#include <iomanip>
#include <vector>
#include "SymTab.hpp"


void SymTab::setValueFor(std::string vName, int value) {
    scope.back()[vName] = TypeDescriptor(value);
}

void SymTab::setValueFor(std::string vName, std::string value) {
    scope.back()[vName] = TypeDescriptor(value);
}

void SymTab::setValueFor(std::string vName, bool value) {
    scope.back()[vName] = TypeDescriptor(value);
}

void SymTab::setValueFor(std::string vName, TypeDescriptor value)
{
    scope.back()[vName] = value;
}

bool SymTab::isDefined(std::string vName) {
    return scope.back().find(vName) != scope.back().end();
}

int SymTab::getIntValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    return scope.back().find(vName)->second.get_ivalue();
}

std::string SymTab::getStringValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    return scope.back().find(vName)->second.get_svalue();
}

bool SymTab::getBoolValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    return scope.back().find(vName)->second.get_bvalue();
}

TypeDescriptor SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    return scope.back().find(vName)->second;
}

void SymTab::openScope() {

    std::map <std::string, TypeDescriptor> newScope;

    scope.push_back(newScope);

}

void SymTab::closeScope() {

    scope.pop_back();
}

Function *SymTab::getFunction(std::string name) {


    return functionTab[name];
}

void SymTab::setFunction(std::string name, Function* func) {

    functionTab[name] = func;
}


