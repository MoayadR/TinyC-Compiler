#include "parser.h"
#include<iostream>

Parser::Parser() {
    this->scanner.setFileContent("..\\input.txt");

}

void Parser::PrintTree(TreeNode *node, int sh) {
    int i, NSH=3;
    for(i=0;i<sh;i++) printf(" ");

    printf("[%s]", NodeKindStr[node->node_kind].c_str());

    if(node->node_kind==OPER_NODE) printf("[%s]", TokenTypeStr[node->oper].c_str());
    else if(node->node_kind==NUM_NODE) printf("[%d]", node->num);
    else if(node->node_kind==ID_NODE || node->node_kind==READ_NODE || node->node_kind==ASSIGN_NODE) printf("[%s]", node->id.c_str());

    if(node->expr_data_type!=VOID) printf("[%s]", ExprDataTypeStr[node->expr_data_type].c_str());

    printf("\n");

    for(i=0;i<MAX_CHILDREN;i++) if(node->child[i]) PrintTree(node->child[i], sh+NSH);
    if(node->sibling) PrintTree(node->sibling, sh);
}

TreeNode *Parser::newexpr() { // error handling
    TreeNode* root;
    this->currentToken = this->scanner.getWithoutConsumtion();

    switch (this->currentToken.getType()) {
        case LEFT_PAREN: // mathexpr  to be continued
            this->scanner.getToken();
            root = new TreeNode();

            this->scanner.getToken();
            break;
        case NUM:
            root = new TreeNode();
            this->scanner.getToken();
            root->node_kind = NUM_NODE;
            root->expr_data_type = INTEGER;
            root->num = std::atoi(this->currentToken.getValue().c_str());
            root->line_num = this->currentToken.getLine();
            break;
        case ID:
            root = new TreeNode();
            this->scanner.getToken();
            root->node_kind = ID_NODE;
            root->id = this->currentToken.getValue();
            root->line_num = this->currentToken.getLine();
            break;
        default:
            break;
    }

    return root;
}

TreeNode *Parser::factor() { // error handling
    TreeNode* left  = this->newexpr();
    this->currentToken = this->scanner.getWithoutConsumtion();
    TreeNode* root;
    if( this->currentToken.getType() == POWER) // ^
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind=OPER_NODE;
        root->expr_data_type = INTEGER;
        root->oper = this->currentToken.getType();
        root->line_num = this->currentToken.getLine();
        root->child[0] = left;

        TreeNode *right = factor();

        if(right == nullptr)
            cout<<"Error at Right Tree"<<endl;

        root->child[1] = right;
        return root;
    }

    return left; // newexpr
}

TreeNode *Parser::term() { // error handling
    TreeNode* left = factor();
    this->currentToken = this->scanner.getWithoutConsumtion();
    TreeNode* root;

    while(this-> currentToken.getType() == DIVIDE || this-> currentToken.getType() == TIMES)
    {
        this->currentToken = this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = OPER_NODE;
        root->expr_data_type = INTEGER;
        root->line_num = this->currentToken.getLine();

        root->oper = this->currentToken.getType();

        root->child[0] = left;
        root->child[1] = this->factor();
        left = root;
        this->currentToken = this->scanner.getWithoutConsumtion();
    }

    return left;
}

TreeNode *Parser::mathexpr() { // error handling
    TreeNode* left = this->term();
    this->currentToken = this->scanner.getWithoutConsumtion();
    TreeNode* root;
    while(currentToken.getType() == PLUS || currentToken.getType() == MINUS)
    {
        this->currentToken = this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = OPER_NODE;

        root->oper = this->currentToken.getType();

        root->expr_data_type = INTEGER;
        root->line_num = this->currentToken.getLine();

        root->child[0] = left;
        root->child[1] = this->term();
        left = root;

        this->currentToken = this->scanner.getWithoutConsumtion();
    }

    return left;
}

TreeNode *Parser::expr() {
    TreeNode* left = this->mathexpr();
    this->currentToken = this->scanner.getWithoutConsumtion();
    TreeNode* root;
    if(currentToken.getType() == LESS_THAN || currentToken.getType() == EQUAL) {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = OPER_NODE;

        root->oper = this->currentToken.getType();

        root->expr_data_type = BOOLEAN;
        root->line_num = this->currentToken.getLine();

        root->child[0] = left;
        root->child[1] = this->mathexpr();
        left = root;
    }
    return left;
}

TreeNode *Parser::writestmt() {
    TreeNode* root;
    this->currentToken = this->scanner.getWithoutConsumtion();
    if(this->currentToken.getType() == WRITE)
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = WRITE_NODE;
        root->oper = WRITE;

        root->child[0] = this->expr();
    }
    return root;
}

TreeNode *Parser::idNode() {
    TreeNode* root;
    this->currentToken = this->scanner.getWithoutConsumtion();
    if (this->currentToken.getType() == ID)
    {
        root = new TreeNode();
        this->scanner.getToken();
        root->node_kind = ID_NODE;
        root->id = this->currentToken.getValue();
        root->line_num = this->currentToken.getLine();
    }
    return root;
}

TreeNode *Parser::readstmt() {
    TreeNode* root;
    this->currentToken = this->scanner.getWithoutConsumtion();
    if(this->currentToken.getType() == READ) {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = READ_NODE;
        root->id = this->currentToken.getValue();
        root->child[0] = this->idNode();
    }
    return root;
}

TreeNode *Parser::assignstmt() {
    TreeNode* root;
    TreeNode* right;
    TreeNode* left = this->idNode();

    this->currentToken = this->scanner.getWithoutConsumtion();
    if(left != nullptr && currentToken.getType() == ASSIGN)
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = ASSIGN_NODE;
        root->id = this->currentToken.getValue();

        right = this->expr();

        root->child[0] = left;
        root->child[1] = right;
    }
    return root;
}

TreeNode *Parser::repeatstmt() {
    TreeNode* root;
    this->currentToken = this->scanner.getWithoutConsumtion();
    if (currentToken.getType() == REPEAT)
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = REPEAT_NODE;

        root->child[0] = this->stmtseq();
        root->child[1] = this->untilNode();
        root->child[2] = this->expr();

    }
    return root;
}

TreeNode *Parser::stmtseq(){
    TreeNode* root, *left, *right;
    left = this->stmt();
    this->currentToken = this->scanner.getWithoutConsumtion();
    while(currentToken.getType() == SEMI_COLON)
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = OPER_NODE;
        root->oper = SEMI_COLON;

        right = this->stmt();

        root->child[0] = left;
        root->child[1] = right;

        left = root;
        this->currentToken = this->scanner.getWithoutConsumtion();
    }
    return left;
}

TreeNode *Parser::untilNode() {
    TreeNode* root;
    this->currentToken = this->scanner.getWithoutConsumtion();
    if(currentToken.getType() == UNTIL)
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = OPER_NODE;
        root->oper = UNTIL;
    }
    return root;
}

TreeNode *Parser::ifstmt() {
    TreeNode* root ;
    this->currentToken = this->scanner.getWithoutConsumtion();
    if(currentToken.getType() == IF)
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = IF_NODE;

        root->child[0] = this->expr();

        this->currentToken = this->scanner.getWithoutConsumtion();
        if(currentToken.getType() != THEN) // Error Handling
        {
            cout << "err then";
        }
        this->scanner.getToken();

        root->child[1] = this->stmtseq();

        this->currentToken = this->scanner.getWithoutConsumtion();
        if(currentToken.getType() == ELSE) // Error Handling
        {
            root->child[2] = new TreeNode();
            root->child[2]->node_kind = OPER_NODE;
            root->child[2]->oper = ELSE;
            this->scanner.getToken();
            root->child[2]->child[0] = this->stmtseq();
        }

        this->currentToken = this->scanner.getWithoutConsumtion();
        if(currentToken.getType() != END) // error handling
        {
            cout << "err end";
        }
        this->scanner.getToken();

    }
    return root;
}

TreeNode *Parser::stmt() {
    TreeNode* root;

    this->currentToken = this->scanner.getWithoutConsumtion();
    switch(currentToken.getType())
    {
        case IF:
            root = this->ifstmt();
            break;
        case REPEAT:
            root = this->repeatstmt();
            break;
        case ID: // Assign
            root = this->assignstmt();
            break;
        case READ:
            root = this->readstmt();
            break;
        case WRITE:
            root = this->writestmt();
            break;
        default: // err handling
            break;
    }

    return root;
}

TreeNode *Parser::program() {
    return stmtseq();
}




