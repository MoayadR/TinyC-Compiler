#include "parser.h"
#include<iostream>
#include <math.h>

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
    TreeNode* root = nullptr;
    this->currentToken = this->scanner.getWithoutConsumtion();

    switch (this->currentToken.getType()) {
        case LEFT_PAREN:
            this->scanner.getToken();
            root = new TreeNode();
            root = this->mathexpr();
            this->currentToken = this->scanner.getToken();
            if(this->currentToken.getType() != RIGHT_PAREN) // error handling
                cout<<"ERROR"<<endl;
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
            this->symbolTable.Insert(this->currentToken.getValue().c_str(), this->currentToken.getLine());
            root->node_kind = ID_NODE;
            root->expr_data_type = INTEGER;
            root->id = this->currentToken.getValue();
            root->line_num = this->currentToken.getLine();
            break;
        default:
            break;
    }
    this->checkNull(root);
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

        this->checkNull(right);

        root->child[1] = right;
        return root;
    }

    return left; // newexpr
}

TreeNode *Parser::term() { // error handling
    TreeNode* left = factor();
    this->currentToken = this->scanner.getWithoutConsumtion();
    TreeNode* root = nullptr;

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

        this->checkNull(root->child[1]);

        left = root;
        this->currentToken = this->scanner.getWithoutConsumtion();
    }

    return left;
}

TreeNode *Parser::mathexpr() { // error handling
    TreeNode* left = this->term();
    this->currentToken = this->scanner.getWithoutConsumtion();
    TreeNode* root = nullptr;
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

        this->checkNull(root->child[1]);

        left = root;

        this->currentToken = this->scanner.getWithoutConsumtion();
    }

    return left;
}

TreeNode *Parser::expr() {
    TreeNode* left = this->mathexpr();
    this->currentToken = this->scanner.getWithoutConsumtion();
    TreeNode* root = nullptr;
    if(currentToken.getType() == LESS_THAN || currentToken.getType() == EQUAL) {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = OPER_NODE;

        root->oper = this->currentToken.getType();

        root->expr_data_type = BOOLEAN;
        root->line_num = this->currentToken.getLine();

        root->child[0] = left;
        root->child[1] = this->mathexpr();

        this->checkNull(root->child[1]);


        left = root;
    }
    return left;
}

TreeNode *Parser::writestmt() {
    TreeNode* root = nullptr;
    this->currentToken = this->scanner.getWithoutConsumtion();
    if(this->currentToken.getType() == WRITE)
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = WRITE_NODE;
        root->oper = WRITE;

        root->child[0] = this->expr();
        this->checkNull(root->child[0]);
    }
    return root;
}

TreeNode *Parser::idNode() {
    TreeNode* root = nullptr;
    this->currentToken = this->scanner.getWithoutConsumtion();
    if (this->currentToken.getType() == ID)
    {
        this->symbolTable.Insert(this->currentToken.getValue().c_str(), this->currentToken.getLine());
        root = new TreeNode();
        this->scanner.getToken();
        root->node_kind = ID_NODE;
        root->expr_data_type = INTEGER;
        root->id = this->currentToken.getValue();
        root->line_num = this->currentToken.getLine();
    }
    return root;
}

TreeNode *Parser::readstmt() {
    TreeNode* root = nullptr;
    this->currentToken = this->scanner.getWithoutConsumtion();
    if(this->currentToken.getType() == READ) {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = READ_NODE;
        root->id = this->currentToken.getValue();
        root->child[0] = this->idNode();
        this->checkNull(root->child[0]);
    }
    return root;
}

TreeNode *Parser::assignstmt() {
    TreeNode* root = nullptr;
    TreeNode* right = nullptr;
    TreeNode* left = this->idNode();

    this->currentToken = this->scanner.getWithoutConsumtion();
    if(left != nullptr && currentToken.getType() == ASSIGN)
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = ASSIGN_NODE;
        root->expr_data_type = INTEGER;
        root->id = this->currentToken.getValue();

        right = this->expr();

        this->checkNull(right);

        root->child[0] = left;
        root->child[1] = right;
    }
    return root;
}

TreeNode *Parser::repeatstmt() {
    TreeNode* root = nullptr;
    this->currentToken = this->scanner.getWithoutConsumtion();
    if (currentToken.getType() == REPEAT)
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = REPEAT_NODE;

        root->child[0] = this->stmtseq();
        this->checkNull(root->child[0]);
        root->child[1] = this->untilNode();
        this->checkNull(root->child[1]);
        root->child[2] = this->expr();
        this->checkNull(root->child[2]);

        // type checking boolean for expr node
        if(root->child[2]->expr_data_type != BOOLEAN)
            cout<<"Error, an \"until\" should be followed by a Boolean"<<endl;

    }
    return root;
}

TreeNode *Parser::stmtseq(){
    TreeNode* root = nullptr, *left = nullptr, *right = nullptr;
    left = this->stmt();
    this->checkNull(left);
    this->currentToken = this->scanner.getWithoutConsumtion();
    while(currentToken.getType() == SEMI_COLON)
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = OPER_NODE;
        root->oper = SEMI_COLON;

        right = this->stmt();
        this->checkNull(right);


        root->child[0] = left;
        root->child[1] = right;

        left = root;
        this->currentToken = this->scanner.getWithoutConsumtion();
    }
    return left;
}

TreeNode *Parser::untilNode() {
    TreeNode* root = nullptr;
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
    TreeNode* root = nullptr ;
    this->currentToken = this->scanner.getWithoutConsumtion();
    if(currentToken.getType() == IF)
    {
        this->scanner.getToken();
        root = new TreeNode();
        root->node_kind = IF_NODE;

        root->child[0] = this->expr();

        // type checking for boolean expr
        if(root->child[0]->expr_data_type != BOOLEAN)
            cout<<"Error, an \"if\" should be followed by a Boolean"<<endl;

        this->checkNull(root->child[0]);


        this->currentToken = this->scanner.getWithoutConsumtion();
        if(currentToken.getType() != THEN) // Error Handling
        {
            cout << "ERROR";
        }
        this->scanner.getToken();

        root->child[1] = this->stmtseq();
        this->checkNull(root->child[1]);


        this->currentToken = this->scanner.getWithoutConsumtion();
        if(currentToken.getType() == ELSE) // Error Handling
        {
            root->child[2] = new TreeNode();
            root->child[2]->node_kind = OPER_NODE;
            root->child[2]->oper = ELSE;
            this->scanner.getToken();
            root->child[2]->child[0] = this->stmtseq();
            this->checkNull(root->child[2]->child[0]);

        }

        this->currentToken = this->scanner.getWithoutConsumtion();
        if(currentToken.getType() != END) // error handling
        {
            cout << "ERROR";
        }
        this->scanner.getToken();

    }
    return root;
}

TreeNode *Parser::stmt() {
    TreeNode* root = nullptr;

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

    this->checkNull(root);

    return root;
}

TreeNode *Parser::program() {
    return stmtseq();
}

void Parser::checkNull(TreeNode *node) {
    if(node == nullptr)
        cout<<"ERROR"<<endl;
}

void Parser::printSymbolTable() {
    this->symbolTable.Print();
}

void Parser::codeGeneration(TreeNode* root) {
    if(root == nullptr)
        return;
    TreeNode* id;
    VariableInfo* varInfo;
    switch (root->node_kind) {
        case IF_NODE:
            if(this->evaluate(root->child[0]))
            {
                this->codeGeneration(root->child[1]);
            }
            else // else stmt
            {
                if(root->child[2] != nullptr && root->child[2]->child[0] != nullptr)
                    this->codeGeneration(root->child[2]->child[0]);
            }
            break;
        case READ_NODE:
            id = root->child[0];
            varInfo =  symbolTable.Find(id->id.c_str());
            cout<<"Enter " <<varInfo->name<<':';
            cin>>  varInfo->value;
            break;
        case WRITE_NODE:
            if(root->child[0]->node_kind == ID_NODE)
            {
                id = root->child[0];
                varInfo =  symbolTable.Find(id->id.c_str());
                cout<<varInfo->name<<": "<<varInfo->value<<endl;
            }
            else
            {
                cout<<this->calculateSubTree(root->child[0])<<endl;
            }
            break;
        case REPEAT_NODE:
            do {
                this->codeGeneration(root->child[0]);
            }
            while(!this->evaluate(root->child[2])); // until not condition true
            break;
        case ASSIGN_NODE:
            id = root->child[0];
            varInfo =  symbolTable.Find(id->id.c_str());
            varInfo->value = this->calculateSubTree(root->child[1]);
            break;
        default:
            this->codeGeneration(root->child[0]);
            this->codeGeneration(root->child[1]);
            break;
    }


//    this->codeGeneration(root->child[2]);
}

int Parser::calculateSubTree(TreeNode *root) { // evaluate and get value
    if (root->node_kind == ID_NODE) {
        VariableInfo* varInfo =  symbolTable.Find(root->id.c_str());
        return varInfo->value;
    }
    if(root->node_kind == NUM_NODE)
        return root->num;

    switch (root->oper) { // PLUS, MINUS, TIMES, DIVIDE,
        case PLUS:
            return this->calculateSubTree(root->child[0]) + this->calculateSubTree(root->child[1]);
            break;
        case MINUS:
            return this->calculateSubTree(root->child[0]) - this->calculateSubTree(root->child[1]);
            break;
        case TIMES:
            return this->calculateSubTree(root->child[0]) * this->calculateSubTree(root->child[1]);
            break;
        case DIVIDE:
            return this->calculateSubTree(root->child[0]) / this->calculateSubTree(root->child[1]);
            break;
        case POWER:
            return pow(this->calculateSubTree(root->child[0]) , this->calculateSubTree(root->child[1]));
            break;
        default:
            cout<<"ERROR DURING TREE Evaluation"<<endl;
            return -1;
    }
}

bool Parser::evaluate(TreeNode *root) {  // evaluate boolean
    int left=calculateSubTree(root->child[0])   , right = calculateSubTree(root->child[1]);
    switch (root->oper) { // EQUAL , LESS_THAN,
        case EQUAL:
            return left == right;
            break;
        case LESS_THAN:
            return left < right;
            break;
        default:
            cout << "ERROR DURING TREE Evaluation" << endl;
            return 0;
            break;
    }
}






