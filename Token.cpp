//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'},
                _isWholeNumber{false}, _is_indent{false}, _is_dedent{false} {}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isCloseParen() )  std::cout << ")" ;
    else if( isOpenCurlyBrace() ) std::cout << "{";
    else if( isCloseCurlyBrace() ) std::cout << "}";
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isSemiColon() )  std::cout << ";" ;
    else if( isColon() ) std::cout << ":";
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if( isName() || isKeyword() )  std::cout << getName();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else if( isGreaterThanOperator() ) std::cout << " > ";
    else if( isGreaterThanEqualOperator() ) std::cout << " >= ";
    else if( isLessThanOperator() ) std::cout << " < ";
    else if( isLessThanEqualOperator() ) std::cout << " <= ";
    else if( isComparisonOperator() ) std::cout << " == ";
    else if( isExclamComparisonOperator() ) std::cout << " != ";
    else if( isIndent() ) std::cout << "Indent";
    else if( isDedent() ) std::cout << "Dedent";
    else if( isComma() ) std::cout << ",";
    else if( isOpenSquareBracket() ) std::cout << "[";
    else if( isCloseSquareBracket() ) std::cout << "]";
    else if( isDoubleQuote() ) std::cout << "\"";
    else if( isSingleQuote() ) std::cout << "\'";
    else std::cout << "Uninitialized token.\n";
}
