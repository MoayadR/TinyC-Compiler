#ifndef TINYC_COMPILER_PARSER_H
#define TINYC_COMPILER_PARSER_H
#include "symbolTable.h"
#include "scanner.h"

class Parser {
private:
    Scanner scanner;
    Token currentToken;
    SymbolTable symbolTable;
public:
    Parser();
    void PrintTree(TreeNode* node, int sh=0);
    TreeNode* newexpr();
    TreeNode* factor();
    TreeNode* term();
    TreeNode* mathexpr();
    TreeNode* expr();
    TreeNode* writestmt();
    TreeNode* readstmt();
    TreeNode* idNode();
    TreeNode* assignstmt();
    TreeNode* repeatstmt(); // test
    TreeNode* untilNode();  // test
    TreeNode* stmtseq();    // test
    TreeNode* ifstmt();
    TreeNode* stmt();
    TreeNode* program();
    void checkNull(TreeNode* node);
    void printSymbolTable();
    void codeGeneration(TreeNode* root);

    bool evaluate(TreeNode *root);
    int calculateSubTree(TreeNode* root);

};

////////////////////////////////////////////////////////////////////////////////////
// Parser //////////////////////////////////////////////////////////////////////////

// program -> stmtseq
// stmtseq -> stmt { ; stmt }
// stmt -> ifstmt | repeatstmt | assignstmt | readstmt | writestmt
// ifstmt -> if exp then stmtseq [ else stmtseq ] end
// repeatstmt -> repeat stmtseq until expr
// assignstmt -> identifier := expr
// readstmt -> read identifier
// writestmt -> write expr
// expr -> mathexpr [ (<|=) mathexpr ]
// mathexpr -> term { (+|-) term }    left associative
// term -> factor { (*|/) factor }    left associative
// factor -> newexpr { ^ newexpr }    right associative
// newexpr -> ( mathexpr ) | number | identifier

#endif //TINYC_COMPILER_PARSER_H
