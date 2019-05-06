//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"

std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;
    while( inStream.get(c) && (isalnum(c) || c == '_')) {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return name;
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

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{} {
    indents.push_back(0);
    line_begin = true;
    prepared_dedents = 0;
}

Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }
    if (prepared_dedents > 0) {
        prepared_dedents--;
        Token t = Token();
        t.set_dedent();
        _tokens.push_back(t);
        return lastToken = t;
    }

    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }

    char c;
    int space_count = 0;

    while (inStream.get(c) && (isspace(c) && c != '\n')) {
        space_count++;
    }

    Token token;

    if (inStream.eof()) {
        token.eof() = true;
    } else if (c == '#') {
        while (inStream.get(c) && c != '\n');
        if (inStream.eof()) {
            token.eof() = true;
            return token;
        }
        inStream.unget();
        return getToken();
    } else if (c == '\n') {
        if (line_begin) {
            //Skip empty liens
            return getToken();
        } else {
            line_begin = true;
            token.eol() = true;
            _tokens.push_back(token);
            return lastToken = token;
        }
    } else if (line_begin && space_count > indents.back()) {
        inStream.putback(c);
        indents.push_back(space_count);
        //std::cout << "New indent found of: " << space_count << std::endl;
        token.set_indent();
    } else if (line_begin && space_count < indents.back()) {
        inStream.putback(c);
        indents.pop_back();
        //std::cout << "Dedent found back to: " << space_count << std::endl;
        token.set_dedent();
        for (int i = indents.size() - 1; i >= 0; i--) {
            if (space_count == indents[i]) {
                break;
            }
            if (space_count > indents[i]) {
                std::cout << "Indentation error found in tokenizer" << std::endl;
                exit(1);
            }
            indents.pop_back();
            prepared_dedents++;
        }
    } else if (isdigit(c)) {
        inStream.putback(c);
        token.setWholeNumber( readInteger() );

    } else if (c == '=') {
        char lookahead;
        inStream.get(lookahead);
        if (lookahead == '=') {
            token.symbol("==");
        }
        else {
            inStream.putback(lookahead);
            token.symbol(c);
        }
    }
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        token.symbol(c);
    else if (c == ';' || c == ',' || c == ':' || c == '\"' || c == '\'')
        token.symbol(c);
    else if (c == '(' || c == ')')
        token.symbol(c);
    else if (c == '{' || c == '}')
        token.symbol(c);
    else if (c == '[' || c == ']')
        token.symbol(c);
    else if (c == '>' || c == '<')
    {
        char lookahead;
        inStream.get(lookahead);
        if (lookahead == '=') {
            std::string s;
            if (c == '>') s = ">=";
            else s = "<=";
            token.symbol(s);
        } else {
            inStream.putback(lookahead);
            token.symbol(c);
        }
    }
    else if (c == '!') {
        inStream.get(c);
        if (c != '=') {
            std::cout << "Expected = got: " << c << " instead" << std::endl;
        }
        token.symbol("!=");
    } else if(isalpha(c) || c == '_') {  // Checks to see if we have a name or _name so we can use it in a variable name
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        std::string s = readName();
        token.setName(s);
    } else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        exit(1);
    }

    line_begin = false;
    _tokens.push_back(token);
    return lastToken = token;
}

std::string Tokenizer::read_string() {
    char c;
    std::string res = "";
    while (inStream.get(c) && c != '\"' && c != '\'') {
        res += c;
    }
    inStream.unget();
    return res;
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
