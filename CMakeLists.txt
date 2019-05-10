cmake_minimum_required(VERSION 3.9)
project(ExprInter)

set(CMAKE_CXX_STANDARD 14)

add_executable(ExprInter main.cpp Token.cpp Token.hpp Tokenizer.cpp Tokenizer.hpp Parser.hpp Parser.cpp Expr.cpp Expr.hpp SymTab.cpp SymTab.hpp Statements.cpp Statements.hpp Arguments.cpp Range.cpp TypeDescriptor.cpp Array.cpp Array.h FunctionExpression.cpp FunctionExpression.h Function.cpp Function.h ReturnStatement.cpp ReturnStatement.h)