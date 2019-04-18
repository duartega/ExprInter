//
// Created by Gabriel Duarte on 1/30/19.
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

    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }
    bool isOpenBrace() const { return _symbol == '{'; }
    bool isCloseBrace() const { return _symbol == '}'; }


    bool isForLoop() const {return _name == "for";}
    bool isPrint() const {return _name == "print";}

    void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }


    bool isSemiColon() const { return _symbol == ';'; }
    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }

    bool isEqualOperator() const          { return _relOp == "==";}
    bool isNotEqualOperator() const          { return _relOp == "!=";}
    bool isGt() const  { return _relOp == ">"; }
    bool isGte() const  { return _relOp == ">="; }
    bool isLt() const  { return _relOp == "<"; }
    bool isLte() const  { return _relOp == "<="; }

    bool isComma() const { return _symbol == ','; }

    bool isQuotations() const { return _symbol == '"'; }
    bool isSingleQuote() const { return _symbol == '\''; }
    bool isCommentSymbol() const { return _symbol == '#'; }


    bool isRelationalOperator() const {
        return isGt() || isGte() || isLt() || isLte();
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

//    bool isRelOp() const { return _relOp.length() > 0; }
//    std::string getStr() { return _relOp; }
//    void setStr(std::string s) { _relOp = s; }

    bool isString() const { return _string.length() > 0; }
    std::string getString() const { return _string; }
    void setString(std::string s) { _string = s; _isString = true; }




    bool isKeyword() const{
        return isForLoop() || isPrint();
    }


    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }

    void print() const;

private:
    std::string _name;
    bool _eof, _eol;
    bool _isWholeNumber;
    bool _isString;
    char _symbol;
    std::string _relOp;
    int _wholeNumber;
    std::string _keyword;
    std::string _string;

};

#endif //EXPRINTER_TOKEN_HPP
