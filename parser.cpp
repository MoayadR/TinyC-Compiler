
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
    TreeNode* root = new TreeNode();
    this->currentToken = this->scanner.getWithoutConsumtion();

    switch (this->currentToken.getType()) {
        case LEFT_PAREN: // mathexpr  to be continued
            this->scanner.getToken();


            this->scanner.getToken();
            break;
        case NUM:
            this->scanner.getToken();
            root->node_kind = NUM_NODE;
            root->expr_data_type = INTEGER;
            root->num = std::atoi(this->currentToken.getValue().c_str());
            root->line_num = this->currentToken.getLine();
            break;
        case ID:
            this->scanner.getToken();
            root->node_kind = ID_NODE;
            root->expr_data_type = INTEGER;
            root->id = this->currentToken.getValue();
            root->line_num = this->currentToken.getLine();
            break;
        default:
            return nullptr;
            break;
    }

    return root;
}

TreeNode *Parser::factor() { // error handling
    TreeNode* left  = this->newexpr();
    this->currentToken = this->scanner.getWithoutConsumtion();
    TreeNode* root;
    if( this->currentToken.getValue() == "^") // ^
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind=OPER_NODE;
        root->expr_data_type = INTEGER;
        root -> oper = POWER;
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

    while(this-> currentToken.getValue() == "/" || this-> currentToken.getValue() == "*")
    {
        this->currentToken = this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = OPER_NODE;
        root->expr_data_type = INTEGER;
        root->line_num = this->currentToken.getLine();

        if ( this->currentToken.getValue() == "*")
            root->oper = TIMES;
        else
            root->oper = DIVIDE;

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
    while(currentToken.getValue() == "+" || currentToken.getValue() == "-")
    {
        this->currentToken = this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = OPER_NODE;

        if ( this->currentToken.getValue() == "+")
            root->oper = PLUS;
        else
            root->oper = MINUS;

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
    while(currentToken.getValue() == "<" || currentToken.getValue() == "=") {
        this->currentToken = this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = OPER_NODE;

        if ( this->currentToken.getValue() == "<")
            root->oper = LESS_THAN;
        else
            root->oper = EQUAL;

        root->expr_data_type = BOOLEAN;
        root->line_num = this->currentToken.getLine();

        root->child[0] = left;
        root->child[1] = this->mathexpr();
        left = root;

        this->currentToken = this->scanner.getWithoutConsumtion();
    }

    return left;

}

TreeNode *Parser::writestmt() {
    this->currentToken = this->scanner.getWithoutConsumtion();
    if(this->currentToken.getValue() == "write")
    {
        this->scanner.getToken();
        TreeNode* root = new TreeNode();
        root->node_kind = WRITE_NODE;
        root->oper = WRITE;

        root->child[0] = this->expr();
        return root;
    }
    return nullptr;
}


