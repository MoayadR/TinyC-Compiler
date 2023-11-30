#include <iostream>
#include "parser.h"




int main() {
    Parser parser;
    parser.PrintTree(parser.writestmt() ,0 );

    std::cout << "Hello, World!" << std::endl;
    return 0;
}