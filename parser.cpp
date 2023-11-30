
#include "parser.h"


Parser::Parser() {
    this->scanner.setFileContent("..\\input.txt");

}

void Parser::PrintTree(TreeNode *node, int sh) {
    int i, NSH=3;
    for(i=0;i<sh;i++) printf(" ");

    printf("[%s]", NodeKindStr[node->node_kind].c_str());

    if(node->node_kind==OPER_NODE) printf("[%s]", TokenTypeStr[node->oper].c_str());
    else if(node->node_kind==NUM_NODE) printf("[%d]", node->num);
    else if(node->node_kind==ID_NODE || node->node_kind==READ_NODE || node->node_kind==ASSIGN_NODE) printf("[%s]", node->id);

    if(node->expr_data_type!=VOID) printf("[%s]", ExprDataTypeStr[node->expr_data_type].c_str());

    printf("\n");

    for(i=0;i<MAX_CHILDREN;i++) if(node->child[i]) PrintTree(node->child[i], sh+NSH);
    if(node->sibling) PrintTree(node->sibling, sh);
}
