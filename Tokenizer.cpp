//
// Created by Gabriel Duarte on 1/30/19.
//

#include <iostream>
#include <string>
#include <stack> // Creating a stack: stack<int> first;
#include "Tokenizer.hpp"


std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;
    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return name;
}

std::string Tokenizer::readString(char quote) {
    // This function is called when it is known that
    // we have a string since we have encountered either a single
    // or double quote. Now, we need to read in the full string.
    // We stop when we reach the matching quote mark.

    std::string stringValue;
    char c;
    while(inStream.get(c) && c != quote){
        stringValue += c;
    }
    return stringValue;
}

int Tokenizer::readInteger() {
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.

    int intValue = 0;
    char c;
    while( inStream.get(c) && isdigit(c) ) {
        intValue = intValue * 10 + c - '0';
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return intValue;
}

std::string Tokenizer::relOp() {
    std::string relOp;
    char c;
    while ( inStream.get(c) && !isspace(c) )
        relOp += c;
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return relOp;
}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{} {}

Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    char c;
    std::string s;

    while( inStream.get(c) && isspace(c)  && c != '\n')  // Skip spaces but not new-line chars.
        ;

    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }
    if (c == '#') {
        while(inStream.get(c) && c != '\n')
            ;
    }

    Token token;
    if( inStream.eof()) {
        token.eof() = true;
    }
    else if( c == '\n' ) {
        token.eol() = true;
        while (inStream.get(c))
        {
            if (c != '\n')
            {
                inStream.putback(c);
                break;
            }
        }
    } else if( isdigit(c) ) { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        token.setWholeNumber(readInteger());
    } else if ( c == '>') {
        if(inStream.peek() == '=') {
            inStream.get(c);
            token.compOp(">=");
        } else
            token.compOp(">");
    } else if (c == '<') {
        if(inStream.peek() == '=') {
            inStream.get(c);
            token.compOp("<=");
        } else
            token.compOp("<");
    } else if (c == '!') {
        if(inStream.peek() == '=') {
            inStream.get(c);
            token.compOp("!=");
        } else
            token.compOp("!");
    } else if( c == '=' ) {
        if(inStream.peek() == '=') {
            inStream.get(c);
            token.compOp("!=");
        } else
            token.compOp("=");
    } else if ( c == ',' )
        token.symbol(c);
    else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        token.symbol(c);
    else if( c == ';' )
        token.symbol(c);
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if(c == '{' || c == '}')
        token.symbol(c);
    else if( c == '"' || c == '\''){
        token.setString( readString(c) );
        token.symbol(c);
    }
    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        token.setName( readName() );
    } else {
        std::cout << "Unknown character in input. -> " << c << " <-" << std::endl;
        exit(1);
    }
    _tokens.push_back(token);
    return lastToken = token;
}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        std::cout << std::endl;
    }
}
