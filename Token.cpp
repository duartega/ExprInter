//
// Created by Gabriel Duarte on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false} , _relOp{""}, _keyword{""},
 _isString{false}, _string{""} {}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isCloseParen() )  std::cout << ")" ;
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isSemiColon() )  std::cout << ";" ;
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if( isName() )  std::cout << getName();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else if( isEqualOperator() ) std::cout << " == ";
    else if( isNotEqualOperator() ) std::cout << " != ";
    else if( isGt() ) std::cout << " > ";
    else if( isGte() ) std::cout << " >= ";
    else if( isLt() ) std::cout << " < ";
    else if( isLte() ) std::cout << " <= ";
    else if ( isOpenBrace() ) std::cout << " {";
    else if ( isCloseBrace() ) std::cout << "} ";
    else if ( isString() ) std::cout << getString();
    else if ( isComma() ) std::cout << " , ";
    //else if( isKeyword() ) std::cout << getKeyword();
    else std::cout << "Token not in Token.cpp.\n";
}
