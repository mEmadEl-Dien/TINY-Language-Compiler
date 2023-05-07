#ifndef SCANNER_H
#define SCANNER_H

#include "includes.h"

enum TokenType
{
    SEMICOLON,
    IF,
    THEN,
    END,
    REPEAT,
    UNTIL,
    IDENTIFIER,
    ASSIGN,
    READ,
    WRITE,
    LESS_THAN,
    EQUAL,
    GREATER_THAN,
    PLUS,
    MINUS,
    MULT,
    DIV,
    OPENBRACKET,
    CLOSEDBRACKET,
    NUMBER,
    ENDOFFILE
};

struct Token
{
    TokenType type;
    string value;
};

class Scanner
{
    private:
        string input, comment;
        int position;
        int TokenCounter;
        char specialSymbols[12] = {';', '<', '=', '>', '+', '-', '*', '/', '(', ')', ':', ' '};

        void Remove_Comments();

        int Is_Special_Symbol(char c);

    public:
        /* Constructor to initialize the private attributes */
        Scanner(string input);

        Token Get_Next_Token();
};

#endif