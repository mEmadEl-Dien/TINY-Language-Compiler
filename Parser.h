#include "Scanner.h"

class ParsingNode
{
    public:
        Token token;
        ParsingNode* sibling;
        ParsingNode* children[4];
        int childrenCounter;

        ParsingNode(Token token):token(token), childrenCounter(0)
        {
            sibling = nullptr;

            for(int i = 0; i < 4; i++)
            {
                children[i] = nullptr;
            }
        }

        void Set_Sibling(ParsingNode* node)
        {
            sibling = node;
        }

        void Set_Child(ParsingNode* node)
        {
            children[childrenCounter++] = node;
        }

        void Set_Node_Token(Token token)
        {
            this->token = token;
        }
};


class Parser
{
    private:
        Scanner scanner;
        Token currentToken;
        ParsingNode* currentNode;

    public:
        ParsingNode* root;

        Parser(Scanner scanner):scanner(scanner), currentNode(nullptr), root(nullptr){}

        int Match(TokenType tokenType)
        {
            if(currentToken.type == tokenType)
            {
                currentToken = scanner.Get_Next_Token();
                return 1;
            }

            return 0;
        }

        void Parse()
        {
            ParsingNode node = Stmt_Sequence();
        }

        ParsingNode Stmt_Sequence()
        {
            currentToken = scanner.Get_Next_Token();

            ParsingNode node = statement();

            cout<<"Read Node: "<<node.token.value<<" "<<node.children[0]->token.value<<endl;
            //cout<<currentToken.value<<endl;

            if(node.token.type == ENDOFFILE)
            {
                cout<<"No Syntax to work on"<<endl;
                exit(EXIT_SUCCESS);
            }

            if(root == nullptr)
                root = &node;

            currentNode = &node;
            
            while(currentToken.type == SEMICOLON)
            {
                Match(SEMICOLON);

                if(currentToken.type == ENDOFFILE)
                    break;

                ParsingNode siblingNode = statement();
                currentNode->Set_Sibling(&siblingNode);
                currentNode = &siblingNode;
            }

            return node;
        }

        ParsingNode statement()
        { 
            ParsingNode readNode({ENDOFFILE, " "});

            if(currentToken.type == IF)
                return if_stmt();

            else if(currentToken.type == REPEAT)
                return repeat_stmt();     

            else if(currentToken.type == IDENTIFIER)
                return assign_stmt();

            else if(currentToken.type == READ)
            {
                readNode = read_stmt();
                cout<<"read node: "<<readNode.token.value<<" "<<readNode.children[0]->token.value<<endl;
                return readNode;
            }
                

            else if(currentToken.type == WRITE)
                return write_stmt();

            else if(currentToken.type == ENDOFFILE)
            {
                ParsingNode endNode({ENDOFFILE, " "});
                return endNode;
            }

            else
            {
                cout<<"Error: Expected (if, repeat, identifier, read, write) in statement"<<endl;
                exit(EXIT_FAILURE);
            }
        }

        ParsingNode if_stmt()
        {
            ParsingNode node({IF, "if"});
            ParsingNode testChild({ENDOFFILE, " "});
            ParsingNode thenChild({ENDOFFILE, " "});

            testChild = exp();
            currentToken = scanner.Get_Next_Token();

            if(Match(THEN))
            {
                thenChild = Stmt_Sequence();
            }

            else
            {
                cout<<"Error: Expected (then) in if-stmt"<<endl;
                exit(EXIT_FAILURE);
            }
        }

        ParsingNode exp()
        {
            ParsingNode node = simple_exp();

            if(currentToken.type == LESS_THAN || currentToken.type == GREATER_THAN || currentToken.type == EQUAL)
            {
                simple_exp();
            }

            return node;
        }

        ParsingNode simple_exp()
        {
            char flag = 0;
            ParsingNode node = term();
            ParsingNode addOpNode({ENDOFFILE, " "});

            while(currentToken.type == PLUS || currentToken.type == MINUS)
            {
                flag = 1;

                ParsingNode addOpNode({currentToken.type, currentToken.value});
                
                if(currentToken.type == PLUS)
                    Match(PLUS);
                else
                    Match(MINUS);

                addOpNode.Set_Child(&node);
                ParsingNode temp = term();
                addOpNode.Set_Child(&temp);
            }

            if(flag)
                node = addOpNode;

            return node;
        }

        ParsingNode term()
        {
            char flag = 0;
            ParsingNode node = factor();
            ParsingNode multiplicationNode({ENDOFFILE, " "});

            while(currentToken.type == MULT)
            {
                flag = 1;
                ParsingNode multiplicationNode({MULT, currentToken.value});
                Match(MULT);

                multiplicationNode.Set_Child(&node);
                ParsingNode temp = factor();
                multiplicationNode.Set_Child(&temp);
            }

            if(flag)
                node = multiplicationNode;

            return node;
        }

        ParsingNode factor()
        {
            ParsingNode node({ENDOFFILE, " "});

            if(currentToken.type == OPENBRACKET)
            {
                Match(OPENBRACKET);
                node = exp();
                Match(CLOSEDBRACKET);
            }

            else if(currentToken.type == NUMBER)
            {
                node.Set_Node_Token({NUMBER, currentToken.value});
                Match(NUMBER);
            }

            else if(currentToken.type == IDENTIFIER)
            {
                node.Set_Node_Token({IDENTIFIER, currentToken.value});
                Match(IDENTIFIER);
            }

            else
            {
                cout<<"Error: Expected (openbracket | number | identifier)"<<endl;
                exit(EXIT_FAILURE);
            }
            
            return node;
        }

        ParsingNode repeat_stmt()
        {

        }

        ParsingNode assign_stmt()
        {
            ParsingNode assignmentNode({IDENTIFIER, currentToken.value});

            Match(IDENTIFIER);

            if(currentToken.type == ASSIGN)
            {
                Match(ASSIGN);
                ParsingNode temp = exp();
                assignmentNode.Set_Child(&temp);
            }

            else
            {
                cout<<"Error: Expected a (:=) after the (identifier) in assign_stmt()"<<endl;
                exit(EXIT_FAILURE);
            }
        }

        ParsingNode read_stmt()
        {
            Match(READ);
            ParsingNode readNode({READ, "read"});

            if(currentToken.type == IDENTIFIER)
            {
                ParsingNode readChild({currentToken.type, currentToken.value});
                readNode.Set_Child(&readChild);
                Match(IDENTIFIER);
            }

            else
            {
                cout<<"Error: Expected an (identifier)"<<endl;
                exit(EXIT_FAILURE);
            }
            cout<<"read node: "<<readNode.token.value<<" "<<readNode.children[0]->token.value<<endl;

            return readNode;
        }

        ParsingNode write_stmt()
        {
            Match(WRITE);

            ParsingNode writeNode({WRITE, "write"});
            ParsingNode temp = exp();

            writeNode.Set_Child(&temp);

            return writeNode;
        }
};