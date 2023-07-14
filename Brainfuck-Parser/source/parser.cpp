#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stack>

#include "parser.hpp"

//Lexer for brainfuck is simple -- just split all chars to token vector
std::vector<Token> Lexer(std::string source)
{
    std::vector<Token> result;

    for (auto c : source)
    {
        try
        {
            result.push_back((Token)c);
        }
        catch (...)
        {
            //Brainfuck ignoring non-syntax characters - try-catch for enum type cast errors
        }
    }

    return result;
}

Commands Parser(std::vector<Token> program)
{
    Commands program_tree;

    for (std::vector<Token>::iterator cmd = program.begin(); cmd != program.end(); cmd++)
    {

        if (*cmd == Token::t_LOOP_OPEN ) {
            //1. find second bracket
            std::vector<Token>::iterator second_bracket = FindBracketPair(program, cmd);
            std::vector<Token> loop_program(cmd+1, second_bracket);
            cmd = second_bracket; // just move iterator to end of brackets
            //2.create loop
            Commands d = Parser(loop_program);
            //3. add loop to commands
            program_tree.addCommand(new Loop(d));
        } else {
            program_tree.addCommand(new Command(*cmd));
        }
    }

    return program_tree;
}

void BuildAST(Commands program){
    //create empty doc
    pugi::xml_document doc;

    //create declaration node
    pugi::xml_node declaration = doc.append_child(pugi::node_declaration);
    declaration.append_attribute("version") = "1.0";

    //create main node
    pugi::xml_node root = doc.append_child("program");
    
    // just fill main node with commands
    for (auto c: program.getCommands()){
        c->appendXML(root);
    }
    
    doc.print(std::cout);
}

std::string ReadFile(char *source_file)
{
    std::ifstream bf_source(source_file);
    std::stringstream buffer;

    if (bf_source.is_open())
    {
        buffer << bf_source.rdbuf();
    }
    else
    {
        std::cerr << "[!] Error: Source Code File can't be open." << std::endl;
        exit(1);
    }

    return buffer.str();
}

bool CheckBracketsCount(std::string source)
{
    if (std::count(source.begin(), source.end(), '[') != std::count(source.begin(), source.end(), ']'))
        return false;
    return true;
}

//as position use bracket that you want to find pair for <3
std::vector<Token>::iterator FindBracketPair(std::vector<Token> source, std::vector<Token>::iterator position)
{
    std::stack< std::vector<Token>::iterator > brackets;

    for (auto i = position; i != source.end(); i++)
    {
        if (*i == Token::t_LOOP_OPEN)
        {
            brackets.push(i);
        }
        if (*i == Token::t_LOOP_CLOSE)
        {
            brackets.pop();       //just pop element because we know position of left bracket

            if (brackets.empty()) //if stack empty - return position of right bracket
                return i;
        }
    }
    return source.end(); //wtf
}

void PrintTree(Commands cmd, int rec_depth){
    for (auto c: cmd.getCommands()){
        if (c->getToken() != Token::t_LOOP){
            std::cout<< std::string(rec_depth, '+') << c->getTokenString()<<std::endl;
        } else {
            PrintTree(dynamic_cast<Loop*>(c)->getCommands(), rec_depth+1);
        }
    }
}