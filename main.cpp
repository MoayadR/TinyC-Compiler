#include <iostream>
#include "parser.h"




int main() {
    Parser parser;
    TreeNode* AST = parser.program();
    parser.PrintTree(AST ,0 );
    cout<<"============================================================"<<endl;
    parser.printSymbolTable();
    parser.codeGeneration(AST);
    return 0;
}