#include "Scanner.h"

/* Constructor to initialize the private attributes */
Scanner::Scanner(string input): input(input), position(0), TokenCounter(0)
{
    /* Removing comments before we start to get Tokens */
    Remove_Comments();

    /* Converting the input string to lowercase */
    transform(input.begin(), input.end(), input.begin(), ::tolower);
}

void Scanner::Remove_Comments()
{
    string output = "";

    for(int i = 0; i < input.length(); i++)
    {
        if(input[i] == '{')
        {
            while(input[i] != '}' && i < input.length())
            {
                comment += input[i];
                i++;
            }
            comment += '}';
        }
        else
        {
            output += input[i];
        }
    }
    
    this->input = output;
}

int Scanner::Is_Special_Symbol(char c)
{
    for (int i = 0; i < 12; i++)
    {
        if (c == specialSymbols[i])
        {
            return 1;
        }
    }

    return 0;
}

Token Scanner::Get_Next_Token()
{
    while(position < input.size() && isspace(input[position]))
    {
        position++;
    }

    if(position >= input.size())
    {
        return {ENDOFFILE, " "};
    }

    char c = input[position];
    position++;

    /* Checking the coming character whether it's a digit, an alphabetic, a special symbol, or neither */
    if(isdigit(c))
    {
        int value = c - '0';
        string valueString = "";
        TokenCounter++;

        while(position < input.size() && isdigit(input[position]))
        {
            value = value * 10 + (input[position] - '0');
            position++;
        }

        valueString = to_string(value);

        return {NUMBER, valueString};
    }

    if(isalpha(c))
    {
        string value = "";
        value += c;
        TokenCounter++;

        while(position < input.size() && !Is_Special_Symbol(input[position]))
        {
            value += input[position];
            position++;
        }

        if(value == "if")
            return {IF, "if"};

        if(value == "then")
            return {THEN, "then"};
        
        if(value == "end")
            return {END, "end"};
        
        if(value == "repeat")
            return {REPEAT, "repeat"};

        if(value == "until")
            return {UNTIL, "until"};

        if(value == "read")
            return {READ, "read"};

        if(value == "write")
            return {WRITE, "write"};
            
        else
            return {IDENTIFIER, value};
    }

    TokenCounter++;

    switch(c)
    {
    case ';':
        return {SEMICOLON, ";"};

    case '<':
        return {LESS_THAN, "<"};

    case '=':
        return {EQUAL, "="};

    case '>':
        return {GREATER_THAN, ">"};

    case '+':
        return {PLUS, "+"};

    case '-':
        return {MINUS, "-"};

    case '*':
        return {MULT, "*"};

    case '/':
        return {DIV, "/"};

    case '(':
        return {OPENBRACKET, "("};

    case ')':
        return {CLOSEDBRACKET, ")"};

    case ':':
        position++;
        return {ASSIGN, ":="};

    default:
        return {ENDOFFILE, " "};
    }
}