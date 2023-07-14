#ifndef AST_PARSER_HPP
#define AST_PARSER_HPP

#include "types.hpp"

/*
  main functionality
*/
std::vector<Token> Lexer(std::string);
Commands Parser(std::vector<Token>);
void BuildAST(Commands);

/*
    util functions
*/
std::string ReadFile(char *);
bool CheckBracketsCount(std::string);
std::vector<Token>::iterator FindBracketPair(std::vector<Token> source, std::vector<Token>::iterator position);
void PrintTree(Commands cmd, int rec_depth);

#endif