//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include<string>

class Token {

public:
    Token();

    bool &eof()  { return _eof; }
    bool &eol()  { return _eol; }

    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }

    bool isOpenParen() const  { return _symbol == "("; }
    bool isCloseParen() const { return _symbol == ")"; }
    bool isOpenCurlyBrace() const {return _symbol == "{";}
    bool isCloseCurlyBrace() const {return _symbol == "}";}

    void symbol(std::string s) { _symbol = s; }
    void symbol(char c) { _symbol = c; }
    std::string symbol() { return _symbol; }

    bool isSemiColon() const { return _symbol == ";"; }
    bool isAssignmentOperator() const              { return _symbol == "="; }
    bool isMultiplicationOperator() const { return _symbol == "*"; }
    bool isAdditionOperator() const       { return _symbol == "+"; }
    bool isSubtractionOperator() const    { return _symbol == "-"; }
    bool isModuloOperator() const         { return _symbol == "%"; }
    bool isDivisionOperator() const       { return _symbol == "/"; }
    bool isGreaterThanOperator() const {return _symbol == ">";}
    bool isGreaterThanEqualOperator() const {return _symbol == ">=";}
    bool isLessThanOperator() const {return _symbol == "<";}
    bool isLessThanEqualOperator() const {return _symbol == "<=";}
    bool isComparisonOperator() const {return _symbol == "==";}
    bool isExclamComparisonOperator() const {return _symbol == "!=";}
    bool isDoubleQuote() const {return _symbol == "\"";}
    bool isSingleQuote() const {return _symbol == "\'";}
    bool isIndent() const {return _is_indent; }
    bool isDedent() const {return _is_dedent; }
    bool isComma() const {return _symbol == ",";}
    bool isColon() const {return _symbol == ":";}
    bool isOpenSquareBracket() const {return _symbol == "[";}
    bool isCloseSquareBracket() const {return _symbol == "]";}
    bool isAnd() const {return _name == "and";}
    bool isOr() const {return _name == "or";}
    bool isNot() const {return _name == "not";}
    bool isKeyword() const
    {
        return _name == "for" || _name == "print" || _name == "if" || _name == "else" ||
               _name == "elif" || _name == "False" || _name == "True" || _name == "def" || _name == "return";
    }

    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }
    bool isName() const                   { return _name.length() > 0; }
    std::string getName() const                  { return _name; }
    void setName(std::string n) { _name = n; }

    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }

    void set_indent() { _is_indent = true;}
    void set_dedent() { _is_dedent = true; }

    void print() const;

private:
    std::string _name;
    bool _eof, _eol;
    bool _isWholeNumber;
    std::string _symbol;
    int _wholeNumber;
    bool _is_dedent, _is_indent;
};

#endif //EXPRINTER_TOKEN_HPP
