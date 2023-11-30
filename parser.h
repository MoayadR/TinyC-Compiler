
#ifndef TINYC_COMPILER_PARSER_H
#define TINYC_COMPILER_PARSER_H

#include "scanner.h"

class Parser {
private:
    Scanner scanner;
    Token currentToken;
public:
    Parser();
    void PrintTree(TreeNode* node, int sh=0);
    TreeNode* newexpr();
    TreeNode* factor();
    TreeNode* term();
    TreeNode* mathexpr();
    TreeNode* expr();
    TreeNode* writestmt();
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
