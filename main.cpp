#include <iostream>
#include "parser.h"




int main() {
    Parser parser;
    parser.PrintTree(parser.program() ,0 );
    return 0;
}