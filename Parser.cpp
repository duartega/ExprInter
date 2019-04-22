
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
#include "SymTab.hpp"



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
    // a series of statements.

    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();

    while (tok.isName() || tok.isKeyword() ) {
        tokenizer.ungetToken();
        if (tok.isKeyword()) {
            if (tok.isPrint()) {
                Print *print = printStatement();
                stmts->addStatement(print);
                tok = tokenizer.getToken();
            }
            else if (tok.isForLoop()){

                For *forLoop = forStatement();
                stmts->addStatement(forLoop);
                tok = tokenizer.getToken();
            }
        } else {
            AssignmentStatement *assignStmt = assignStatement();
            stmts->addStatement(assignStmt);
            tok = tokenizer.getToken();
        }
    }

    tokenizer.ungetToken();
    return stmts;
}

AssignmentStatement *Parser::assignStatement() {
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    ExprNode *rightHandSideExpr = compOp();

    // if we are assigning a string then we need to catch the quotes
    Token quotes = tokenizer.getToken();
    if (quotes.isQuotations()) {
        Token newLine = tokenizer.getToken();
        if (!newLine.eol())
            die ("Parser::assignStatement", "Expected a newline token, instead got", newLine);
    }
    else if (!quotes.eol()) // Check if there is a new line since its not a string
        die ("Parser::assignStatement", "Expected a newLine token, instead got", quotes);

    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

Print *Parser::printStatement() {
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::printStatement", "Expected a name token, instead got", varName);
    if (!varName.isKeyword())
        die("Parser::printStatement", "Expected a 'Print' token, instead got", varName);

    // Check to see if we have commas ( a testlist )
    ExprNode *rightHandSideExpr = testList();

    Token newLine = tokenizer.getToken();
    if (!newLine.eol()) // Check if there is a new line
        die ("Parser::printStatement()", "Expected a newLine token, instead got", newLine);

    return new Print(varName.getName(), rightHandSideExpr);
}

ExprNode* Parser::testList(){
    // This function parses the grammar rules:
    // <testlist> -> <test> { ',' <test> }
    ExprNode *left = test();
    Token tok = tokenizer.getToken();

    // I tried to get a vector working but couldn't, instead used infix down below
    std::vector<Token> t;
    std::vector<ExprNode*> e;
    while (tok.isComma()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = test();
        e.push_back(p->right());
        left = p;
//        std::cout << "testList Tally: "; p->print(); std::cout << '\n';
        t.push_back(p->right()->token());

        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return left;
}
//TODO add the symbols for these and do things
ExprNode *Parser::test() {
    // This function parses the grammar rules:
    // <test> -> <or_test>
    ExprNode *rightHandSideExpr = or_test();
}
ExprNode *Parser::or_test() {
    // This function parses the grammar rules:
    // <or_test> -> <and_test> { 'or' <and_test> }
    ExprNode *rightHandSideExpr = and_test();
}
ExprNode *Parser::and_test() {
    // This function parses the grammar rules:
    // <and_test> -> <not_test> { 'and <not_test> }
    ExprNode *rightHandSideExpr = not_test();
}
ExprNode *Parser::not_test() {
    // This function parses the grammar rules:
    // <not_test> -> <comparison>
    ExprNode *rightHandSideExpr = compOp();
}

For *Parser::forStatement() {
    Token varName = tokenizer.getToken(); // keyword FOR

    if (!varName.isKeyword())
        die("Parser::forStatement", "Expected a 'For' token, instead got", varName);

    Token paren = tokenizer.getToken();
    if(!paren.isOpenParen())
        die("Parser::forStatement", "Expected a '(' token, instead got", paren);

    AssignmentStatement *assign1 = assignStatement();


    Token semiColon = tokenizer.getToken();
    if (!semiColon.isSemiColon())
        die("Parser::forStatement", "Expected a ';' token, instead got", semiColon);

    Token newLine = tokenizer.getToken();
    if (!newLine.eol())
        die("Parser::ForStatement", "Expected a new line token after first assignment, instead got", newLine);

    ExprNode *condition = compOp();

    Token semiColon1 = tokenizer.getToken();
    if (!semiColon1.isSemiColon())
        die("Parser::forStatement", "Expected a ';' token, instead got", semiColon1);

    newLine = tokenizer.getToken();
    if (!newLine.eol())
        die("Parser::ForStatement", "Expected a new line token after condition statement, instead got", newLine);

    AssignmentStatement *assign2 = assignStatement();

    newLine = tokenizer.getToken();
    if (!newLine.eol())
        die("Parser::ForStatement", "Expected a new line token after increment statement, instead got", newLine);

    Token closeParen = tokenizer.getToken();
    if (!closeParen.isCloseParen())
        die("Parser::forStatement", "Expected a ')' token, instead got", closeParen);

    newLine = tokenizer.getToken();
    if (!newLine.eol())
        die("Parser::ForStatement", "Expected a new line token after closing parenthesis, instead got", newLine);


    Token openBrace = tokenizer.getToken();
    if (!openBrace.isOpenBrace())
        die("Parser::forStatement", "Expected a '{' token, instead got", openBrace);

    newLine = tokenizer.getToken();
    if (!newLine.eol())
        die("Parser::ForStatement", "Expected a new line token, instead got", newLine);

    Statements *stmt = statements();

    Token closeBrace = tokenizer.getToken();

    if (!closeBrace.isCloseBrace())
        die("Parser::forStatement", "#2 Expected a '}' token, instead got", closeBrace);
    newLine = tokenizer.getToken();
    if (!newLine.eol())
        die("Parser::ForStatement", "Expected an end of file token, instead got", newLine);
    return new For(assign1, condition, assign2, stmt);
}

ExprNode *Parser::compOp() {
    // This function parses the grammar rules:
    // <relExpr> -> <rel-term> { (==, !=) <rel-term> }
    ExprNode *left = expr();
    Token tok = tokenizer.getToken();
    while (tok.isEqualOperator() || tok.isNotEqualOperator() || tok.isGt() || tok.isGte() || tok.isLt() || tok.isLte())
    {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = expr(); // Made the change to go to expr instead of relTerm from P2 #2
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

//ExprNode *Parser::relTerm() {
//    // This function parses the grammar rules:
//
//    // <relTerm> -> <rel-Primary> { (>, >=, <, <=) <rel-Primary> }
//    ExprNode *left = expr();
//    Token tok = tokenizer.getToken();
//    while (tok.isGt() || tok.isGte() || tok.isLt() || tok.isLte() || tok.isEqualOperator() || tok.isNotEqualOperator())
//    {
//        InfixExprNode *p = new InfixExprNode(tok);
//        p->left() = left;
//        p->right() = expr();
//        left = p;
//        tok = tokenizer.getToken();
//    }
//    tokenizer.ungetToken();
//    return left;
//}

ExprNode *Parser::expr() {
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
//        std::cout << "Expr Tally: "; p->print(); std::cout << '\n';
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


ExprNode *Parser::term() {
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = primary();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::primary() {
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z]+
    // <primary> -> (<expr>)
    // <primary> -> (<rel-expr>)

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber() )
        return new WholeNumber(tok);
    else if( tok.isName() || tok.isQuotations() || tok.isSingleQuote())
        return new Variable(tok);
    else if (tok.isOpenParen()) {
        ExprNode *p = compOp();
        Token token = tokenizer.getToken();

        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    else if (tok.isRelationalOperator())
    {
        ExprNode *p = compOp();
        Token token = tokenizer.getToken();
        return p;
    }

    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
