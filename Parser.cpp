
/*  The majority of the work is done by the class 'convert'.
    This class builds an expression tree using the input infix
    expression.  A post-order traversal of the expression tree 'dumps'
    it into an array in postfix form.  The iterator copies the token
    from this array to user's arrays.

*/

#include<vector>
#include<iostream>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::statements() {
    // This function is called when we KNOW that we are about to parse
    // a series of assorted statements.


    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();
    while (!tok.isDedent() && !tok.eof()) {
        if (tok.isKeyword()) {
            if (tok.getName() == "print") {
                PrintStatement *printstmt = printStatement();
                stmts->addStatement(printstmt);
                tok = tokenizer.getToken();
            } else if (tok.getName() == "for") {
                ForStatement* forstmt = forStatement();
                stmts->addStatement(forstmt);
                tok = tokenizer.getToken();
            } else if (tok.getName() == "if") {
                IfElseStatement* ifstmt = ifElseStatement();
                stmts->addStatement(ifstmt);
                tok = tokenizer.getToken();
            } else if (tok.getName() == "def") {
                FunctionDef * funcDef = functionDef();
                stmts->addStatement(funcDef);
                tok = tokenizer.getToken();


            }
            else if (tok.getName() == "return"){
                ReturnStatement * returnstmt = returnStatement();
                stmts->addStatement(returnstmt);
                tok = tokenizer.getToken();
            }
        } else if (tok.isName()) {

            Token lookahead = tokenizer.getToken();
            if(lookahead.isDotOperator()) // a.pop()
            {
                Token pushOrPop = tokenizer.getToken();
                tokenizer.ungetToken();
                if (pushOrPop.isPop())
                {
                    tokenizer.ungetToken();
                    ArrayPop *popStmt = popStatement(tok);
                    stmts->addStatement(popStmt);
                    tok = tokenizer.getToken();
                } else if (pushOrPop.isPush()) {
                    tokenizer.ungetToken();
                    ArrayPush *pushStmt = pushStatement(tok);
                    stmts->addStatement(pushStmt);
                    tok = tokenizer.getToken();
                }
            } else if (lookahead.isOpenSquareBracket()) {// a[5] = 2
                tokenizer.ungetToken();
                Subscription *sub = subAssign(tok);
                stmts->addStatement(sub);
                tok = tokenizer.getToken();
            }
            else if(lookahead.isOpenParen())
            {
                tokenizer.ungetToken();
                FunctionStatement* funcStmt =  new FunctionStatement(functionNode(tok));

                Token t4 = tokenizer.getToken();
                if (!t4.eol() && !t4.eof()) {
                    die("Parser::functionNode", "Expected new line, instead got", t4);
                }

                stmts->addStatement(funcStmt);
                tok = tokenizer.getToken();



            } else{
                //Assignment statement
                tokenizer.ungetToken();
                AssignmentStatement *assignStmt = assignStatement(tok);
                stmts->addStatement(assignStmt);
                tok = tokenizer.getToken();
            }

        } else {
            die("Parser::statements", "Unexpected token found at start of line", tok);
        }
    }
    tokenizer.ungetToken();
    return stmts;
}
Subscription* Parser::subAssign(Token varName) {

    Token openSB = tokenizer.getToken();
    if (!openSB.isOpenSquareBracket())
        die("Parser::subAssign", "Expected a [ token, instead got", openSB);

    ExprNode* index = rel_expr();

    Token closeSB = tokenizer.getToken();
    if (!closeSB.isCloseSquareBracket())
        die("Parser::subAssign", "Expected a ] token, instead got", closeSB);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::subAssign", "Expected a = , instead got", assignOp);

    ExprNode *assignValue = rel_expr();

    Token t = tokenizer.getToken();
    if (!t.eol() && !t.eof()) {
        die("Parser::subAssign", "Expected a new line, instead got", t);
    }

    return new Subscription(varName, index, assignValue);

}
ArrayPush *Parser::pushStatement(Token varName) {

    Token push = tokenizer.getToken();
    if (!push.isPush())
        die("Parser::pushStatement", "Expected a push token, instead got", push);

    Token openParen = tokenizer.getToken();
    if (!openParen.isOpenParen())
        die("Parser::pushStatement", "Expected an open Parenthesis token, instead got", openParen);

    ExprNode *value = rel_expr();

    Token closeParen = tokenizer.getToken();
    if (!closeParen.isCloseParen())
        die("Parser::pushStatement", "Expected a closed Parenthesis token, instead got", closeParen);

    Token t = tokenizer.getToken();
    if (!t.eol() && !t.eof()) {
        die("Parser::pushStatement", "Expected a new line, instead got", t);
    }
    return new ArrayPush(varName, value);
}
ArrayPop *Parser::popStatement(Token name) {

    Token pop = tokenizer.getToken();
    if (!pop.isName())
        die("Parser::popStatement", "Expected a pop token, instead got", pop);

    Token openParen = tokenizer.getToken();
    if (!openParen.isOpenParen())
        die("Parser::popStatement", "Expected an open Parenthesis token, instead got", openParen);

    Token closeParen = tokenizer.getToken();
    if (!closeParen.isCloseParen())
        die("Parser::popStatement", "Expected a closed Parenthesis token, instead got", closeParen);

    Token t = tokenizer.getToken();
    if (!t.eol() && !t.eof()) {
        die("Parser::popStatement", "Expected a new line, instead got", t);
    }
    return new ArrayPop(name);
}

AssignmentStatement *Parser::assignStatement(Token tok) {
    Token varName = tok;
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    ExprNode *rightHandSideExpr = rel_expr();

    Token t = tokenizer.getToken();
    if (!t.eol() && !t.eof()) {
        die("Parser::assignStatement", "Expected a new line, instead got", t);
    }

    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

PrintStatement* Parser::printStatement() {
    auto p = new PrintStatement(arguments());

    Token t = tokenizer.getToken();
    if (!t.eol() && !t.eof()) {
        die("Parser::printStatement", "Expected a new line, instead got", t);
    }

    return p;
}

Arguments* Parser::arguments() {
    auto v = new std::vector<ExprNode*>;

    v->push_back(rel_expr());
    Token t = tokenizer.getToken();

    while (t.isComma()) {
        v->push_back(rel_expr());
        t = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return new Arguments(v);
}

ForStatement* Parser::forStatement() {
    Token t = tokenizer.getToken();
    if (!t.isName()) {
        die("Parser::forStatement", "Expected a variable name, instead got", t);
    }
    auto *var = new Variable(t);

    Token t2 = tokenizer.getToken();
    if (!t2.isName() || t2.getName() != "in") {
        die("Parser::forStatement", "Expected keyword 'in', instead got", t2);
    }

    Token t3 = tokenizer.getToken();
    if (!t3.isName() || t3.getName() != "range") {
        die("Parser::forStatement", "Expected range, instead got", t3);
    }

    Token t4 = tokenizer.getToken();
    if (!t4.isOpenParen()) {
        die("Parser::forStatement", "Expected open paren, instead got", t4);
    }

    auto *args = arguments();
    if (args->count() == 0 || args->count() > 3) {
        die("Parser::forStatement", "Got wrong number of arguments", t4);
    }
    std::vector<ExprNode*> *v = args->args();
    Range* r;
    if (args->count() == 1) {
        r = new Range(v->at(0));
    } else if (args->count() == 2) {
        r = new Range(v->at(0), v->at(1));
    } else if (args->count() == 3) {
        r = new Range(v->at(0), v->at(1), v->at(2));
    } else {
        std::cout << "What in the world happened, how did you get negative arguments" << std::endl;
        std::cout << args->count();
        exit(1);
    }

    Token t5 = tokenizer.getToken();
    if (!t5.isCloseParen()) {
        die("Parser::forStatement", "Expected close paren, instead got", t5);
    }

    Token t6 = tokenizer.getToken();
    if (!t6.isColon()) {
        die("Parser::forStatement", "Expected colon, instead got", t6);
    }

    Token t7 = tokenizer.getToken();
    if (!t7.eol()) {
        die("Parser::forStatement", "Expected new line, instead got", t7);
    }

    Token t8 = tokenizer.getToken();
    if (!t8.isIndent()) {
        die("Parser::forStatement", "Expected indent, instead got", t8);
    }

    auto stm = statements();

    Token t9 = tokenizer.getToken();
    if (!t9.isDedent() && !t9.eof()) {
        die("Parser::forStatement", "Expected dedent, instead got", t9);
    }

    return new ForStatement(var, r, stm);
}

FunctionDef* Parser::functionDef() {
    Token tok0 = tokenizer.getToken();

    std::string funcName = tok0.getName();
    Token tok1 = tokenizer.getToken();
    if (!tok1.isOpenParen()) {
        die("Parser::functionDef", "Expected open paren, instead got", tok1);
    }

    Arguments *args = nullptr;

    Token tok1a = tokenizer.getToken();
    if(!tok1a.isCloseParen()) {
        tokenizer.ungetToken();
        args = arguments();
    }
    else{
        tokenizer.ungetToken();
    }
    int n;
    if(args)
    {
        n = args->count();
    }
    else
        n = 0;
    std::vector<std::string>params;
    for( int i = 0; i < n; i++)
    {
        ExprNode* expr = args->args()->at(i);
        Token tok = expr->token();
        std::string name = tok.getName();
        params.push_back(name);
    }

    Token t2 = tokenizer.getToken();
    if (!t2.isCloseParen()) {
        die("Parser::functionDef", "Expected close parenthesis, instead got", t2);
    }

    Token t3 = tokenizer.getToken();
    if (!t3.isColon()) {
        die("Parser::functionDef", "Expected colon, instead got", t3);
    }

    Token t4 = tokenizer.getToken();
    if (!t4.eol()) {
        die("Parser::functionDef", "Expected new line, instead got", t4);
    }

    Token t5 = tokenizer.getToken();
    if (!t5.isIndent()) {
        die("Parser::functionDef", "Expected indent, instead got", t5);
    }

    auto stm = statements();

    Token t6 = tokenizer.getToken();
    if (!t6.isDedent() && !t6.eof()) {
        die("Parser::functionDef", "Expected dedent, instead got", t6);
    }

    return new FunctionDef(funcName, params, stm);
}
FunctionNode *Parser::functionNode(Token VarName) {

    std::string funcName = VarName.getName();
    Token tok1 = tokenizer.getToken();
    if (!tok1.isOpenParen()) {
        die("Parser::functionNode", "Expected open paren, instead got", tok1);
    }

    Arguments *args = nullptr;

    Token tok1a = tokenizer.getToken();
    if(!tok1a.isCloseParen()) {
        tokenizer.ungetToken();
        args = arguments();
    }
    else{
        tokenizer.ungetToken();
    }
    int n = 0;
    if(args)
    {
        n = args->count();
    }
    else {
        n = 0;
    }
    std::vector<std::string>params;
    for( int i; i < n; i++)
    {
        ExprNode* expr = args->args()->at(i);
        Token tok = expr->token();
        std::string name = tok.getName();
        params.push_back(name);
    }

    Token t2 = tokenizer.getToken();
    if (!t2.isCloseParen()) {
        die("Parser::functionNode", "Expected close parenthesis, instead got", t2);
    }




    return new FunctionNode(funcName, args);

}

IfElseStatement* Parser::ifElseStatement() {
    Token check;
    auto if_statements = new std::vector<IfStatement*>;
    Statements* potential_else = nullptr;
    do
    {


        ExprNode* cond = rel_expr();

        Token t1 = tokenizer.getToken();
        if (!t1.isColon()) {
            die("Parser::ifElseStatement", "Expected colon, instead got", t1);
        }

        Token t2 = tokenizer.getToken();
        if (!t2.eol()) {
            die("Parser::ifElseStatement", "Expected new line, instead got", t2);
        }

        Token t3 = tokenizer.getToken();
        if (!t3.isIndent()) {
            die("Parser::ifElseStatement", "Expected indent, instead got", t3);
        }

        Statements* stmts = statements();

        Token t4 = tokenizer.getToken();
        if (!t4.isDedent() && !t4.eof()) {
            die("Parser::ifElseStatement", "Expected dedent, instead got", t4);
        }
        check = tokenizer.getToken();
        if_statements->push_back(new IfStatement(cond, stmts));
    }
    while (check.isKeyword() && check.getName() == "elif");

    if (check.isKeyword() && check.getName() == "else") {
        Token t1 = tokenizer.getToken();
        if (!t1.isColon()) {
            die("Parser::ifElseStatement", "Expected colon, instead got", t1);
        }

        Token t2 = tokenizer.getToken();
        if (!t2.eol()) {
            die("Parser::ifElseStatement", "Expected new line, instead got", t2);
        }

        Token t3 = tokenizer.getToken();
        if (!t3.isIndent()) {
            die("Parser::ifElseStatement", "Expected indent, instead got", t3);
        }

        potential_else = statements();

        Token t4 = tokenizer.getToken();
        if (!t4.isDedent() && !t4.eof()) {
            die("Parser::ifElseStatement", "Expected dedent, instead got", t4);
        }
    } else {
        tokenizer.ungetToken();
    }

    return new IfElseStatement(if_statements, potential_else);
}

ExprNode *Parser::rel_expr() {
    //<rel_expr> -> <rel_term> { (==, !=) <rel_term> }
    ExprNode *left = rel_term();
    Token tok = tokenizer.getToken();
    while (tok.isComparisonOperator() || tok.isExclamComparisonOperator()) {
        InfixExprNode *n = new InfixExprNode(tok);
        n->left() = left;
        n->right() = rel_term();
        left = n;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::rel_term() {
    //<rel_term> -> <rel_primary> { (<, <=, >, >=) <rel_primary> }
    ExprNode *left = rel_primary();
    Token tok = tokenizer.getToken();
    while (tok.isGreaterThanOperator() || tok.isLessThanOperator() ||
           tok.isGreaterThanEqualOperator() || tok.isLessThanEqualOperator())
    {
        InfixExprNode *n = new InfixExprNode(tok);
        n->left() = left;
        n->right() = rel_primary();
        left = n;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::rel_primary() {
    ExprNode *left = arith_expr();
    Token tok = tokenizer.getToken();
    while (tok.isAnd() || tok.isOr()) {
        InfixExprNode *n = new InfixExprNode(tok);
        n->left() = left;
        n->right() = arith_expr();
        left = n;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::arith_expr() {
    // This function parses the grammar rules:

    // <arith_expr> -> <term> { <add_op> <arith_term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode *left = arith_term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = arith_term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


ExprNode *Parser::arith_term() {
    // This function parses the grammar rules:

    // <arith_term> -> <primary> { <mult_op> <arith_primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = arith_primary();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = arith_primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::arith_primary() {
    // This function parses the grammar rules:

    // <arith_primary> -> [0-9]+
    // <arith_primary> -> [_a-zA-Z]+
    // <arith_primary> -> (<arith_expr>)

    Token tok = tokenizer.getToken();
    ExprNode* p;
    InfixExprNode* e;

    bool is_negative = false;
    if (tok.isSubtractionOperator()) {
        is_negative = true;
        Token temp1 = Token();
        temp1.symbol("-");
        e = new InfixExprNode(temp1);

        Token temp2 = Token();
        temp2.setWholeNumber(0);
        WholeNumber* n = new WholeNumber(temp2);

        e->left() = n;
        tok = tokenizer.getToken();
    }

    bool is_not = false;
    if (tok.isNot()) {
        is_not = true;
        Token temp1 = Token();
        temp1.symbol("!=");
        e = new InfixExprNode(temp1);

        Token temp2 = Token();
        temp2.setName("True");
        BooleanValue* n = new BooleanValue(temp2);

        e->left() = n;
        tok = tokenizer.getToken();
    }

    if (tok.isKeyword() && (tok.getName() == "True" || tok.getName() == "False")) {
        p = new BooleanValue(tok);
    } else if (tok.isWholeNumber()) {
        p = new WholeNumber(tok);
    } else if (tok.isKeyword() && tok.getName() == "len"){
        Token openParen = tokenizer.getToken();
        if (!openParen.isOpenParen())
            die("Parser::arith_primary", "Expected open-parenthesis, instead got", openParen);

        Token tok = tokenizer.getToken();
        p = new ArraySize(tok);

        Token closeParen = tokenizer.getToken();
        if (!closeParen.isCloseParen())
            die("Parser::arith_primary", "Expected close-parenthesis, instead got", closeParen);

    }else if (tok.isName()) {
        Token lookahead = tokenizer.getToken();
        if(lookahead.isOpenParen())
        {
            tokenizer.ungetToken();
            p=functionNode(tok);

        }
        else if(lookahead.isOpenSquareBracket())
        {
            // left off here for checking a = arr[3] + 5
            p = rel_expr();
            p = new Subscript(tok, p);

            Token closeSB = tokenizer.getToken();
            if (!closeSB.isCloseSquareBracket())
                die("Parser::arith_primary", "Expected close-parenthesis, instead got", closeSB);

        }



        else{
            tokenizer.ungetToken();
            p = new Variable(tok);
        }
    }


    else if (tok.isOpenSquareBracket())
    {
        Token closeSB = tokenizer.getToken();
        if (closeSB.isCloseSquareBracket()) {
            p = new Array(nullptr);
        }
        else {
            tokenizer.ungetToken();
            p = new Array(arguments());

            Token closeSB = tokenizer.getToken();
            if (!closeSB.isCloseSquareBracket())
                die("Parser::arith_primary", "Expected close Square Bracket, instead got", closeSB);
        }
    }
    else if (tok.isOpenParen())
    {

        p = rel_expr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::arith_primary", "Expected close-parenthesis, instead got", token);
    } else if (tok.isDoubleQuote() || tok.isSingleQuote()) {
        Token t = Token();
        t.setName(tokenizer.read_string());
        p = new StringValue(t);
        Token token = tokenizer.getToken();
        if ((tok.isDoubleQuote() && !token.isDoubleQuote()) || (tok.isSingleQuote() && !token.isSingleQuote())) {
            die("Parser::arith_primary", "Expected closing quote, instead got", token);
        }
    } else {
        die("Parser::arith_primary", "Unexpected token", tok);
    }

    if (is_negative || is_not) {
        e->right() = p;
        return e;
    }
    return p;
}

ReturnStatement *Parser::returnStatement() {

    ExprNode* ret = rel_expr();

    Token t4 = tokenizer.getToken();
    if (!t4.eol() && !t4.eof()) {
        die("Parser::functionNode", "Expected new line, instead got", t4);
    }

    return new ReturnStatement(ret);

}


