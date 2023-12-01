//
// Created by moaya on 11/30/2023.
//

#ifndef TINYC_COMPILER_GLOBALS_H
#define TINYC_COMPILER_GLOBALS_H

typedef enum tokenType {
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    ASSIGN, EQUAL, LESS_THAN, GREAT_THAN,
    PLUS, MINUS, TIMES, DIVIDE,
    SEMI_COLON,
    LEFT_PAREN, RIGHT_PAREN,
    ID, NUM,
    ENDFILE, ERROR,WHITESPACE , POWER
} tokenType;

const std::string TokenTypeStr[]=
        {
                "If", "Then", "Else", "End", "Repeat", "Until", "Read", "Write",
                "Assign", "Equal", "LessThan","GreatThan",
                "Plus", "Minus", "Times", "Divide",
                "SemiColon",
                "LeftParen", "RightParen",
                "ID", "Num",
                "EndFile", "Error" , "WhiteSpace" , "POWER"
        };

typedef enum NodeKind{
    IF_NODE, REPEAT_NODE, ASSIGN_NODE, READ_NODE, WRITE_NODE,
    OPER_NODE, NUM_NODE, ID_NODE
}NodeKind;

// Used for debugging only /////////////////////////////////////////////////////////
const std::string NodeKindStr[]=
        {
                "If", "Repeat", "Assign", "Read", "Write",
                "Oper", "Num", "ID"
        };

typedef enum ExprDataType {VOID, INTEGER, BOOLEAN}ExprDataType;

// Used for debugging only /////////////////////////////////////////////////////////
const std::string ExprDataTypeStr[]=
        {
                "Void", "Integer", "Boolean"
        };

#define MAX_CHILDREN 3

struct TreeNode
{
    TreeNode* child[MAX_CHILDREN];
    TreeNode* sibling; // used for sibling statements only

    NodeKind node_kind;

    tokenType oper; int num;std::string id;
    // defined for expression/int/identifier only
    ExprDataType expr_data_type; // defined for expression/int/identifier only

    int line_num;

    TreeNode() {int i; for(i=0;i<MAX_CHILDREN;i++) child[i]=0; sibling=0; expr_data_type=VOID;}
};



#endif // TINYC_COMPILER_GLOBALS_H