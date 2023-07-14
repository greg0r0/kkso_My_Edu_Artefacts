#ifndef AST_TYPES_H
#define AST_TYPES_H

#include <vector>
#include <string>
#include "pugixml.hpp"

//Elements of BF syntax, for lexer parser
enum class Token
{
    t_INCREASE = '+',
    t_DECREASE = '-',
    t_SHIFT_LEFT = '<',
    t_SHIFT_RIGHT = '>',
    t_LOOP_OPEN = '[',
    t_LOOP_CLOSE = ']',
    t_INPUT = ',',
    t_OUTPUT = '.',
    t_LOOP = '!' //unstandart token for AST representation
};

class Command
{
private:
    Token _Token;

public:
    Command() {}
    Command(Token t) : _Token(t) {}

    Token getToken() { return this->_Token; }
    std::string virtual getType() { return "Command"; }

    void virtual appendXML(pugi::xml_node root){
        root.append_child(this->getTokenString().c_str());
    }

    std::string getTokenString()
    {
        switch ((this->_Token))
        {
        case Token::t_INCREASE:
            return "t_INCREASE";
            break;
        case Token::t_DECREASE:
            return "t_DECREASE";
            break;
        case Token::t_SHIFT_LEFT:
            return "t_SHIFT_LEFT";
            break;
        case Token::t_SHIFT_RIGHT:
            return "t_SHIFT_RIGHT";
            break;
        case Token::t_LOOP_OPEN:
            return "t_LOOP_OPEN";
            break;
        case Token::t_LOOP_CLOSE:
            return "t_LOOP_CLOSE";
            break;
        case Token::t_INPUT:
            return "t_INPUT";
            break;
        case Token::t_OUTPUT:
            return "t_OUTPUT";
            break;
        case Token::t_LOOP:
            return "t_LOOP";
            break;
        default:
            return std::string(1, (char)this->_Token);
            break;
        }
    }
};

class Commands
{
private:
    std::vector<Command*> _Commands;
public:
    Commands() {}
    Commands(std::vector<Command*> _c) : _Commands(_c) {}
    void Clear(){
        for (auto c: this->_Commands)
            delete c;
        this->_Commands.clear();
    }
    std::vector<Command*> getCommands() { return this->_Commands;}
    void addCommand(Command* cmd){
        this->_Commands.push_back(cmd);
    }
};

class Loop : public Command
{
private:
    Commands _Commands;

public:
    Loop() {}
    Loop(Commands _c) : Command(Token::t_LOOP), _Commands(_c) {}

    Commands getCommands() { return this->_Commands; }
    std::string getType() { return "Loop"; }

    void virtual appendXML(pugi::xml_node root){
        pugi::xml_node loop = root.append_child("t_LOOP");
        for (auto c: this->_Commands.getCommands())
            c->appendXML(loop);
    }
};

#endif