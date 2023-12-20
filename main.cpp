#include <iostream>
#include "parser.h"




int main() {
    Parser parser;
    TreeNode* AST = parser.program();
    parser.printSymbolTable();
    cout<<"============================================================"<<endl;
    parser.codeGeneration(AST);
    return 0;
}