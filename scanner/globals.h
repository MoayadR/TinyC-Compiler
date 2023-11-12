//
// Created by shadow on 10/31/2023.
//

#ifndef SCANNER_GLOBALS_H
#define SCANNER_GLOBALS_H

typedef enum tokenType {
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    ASSIGN, EQUAL, LESS_THAN, GREAT_THAN,
    PLUS, MINUS, TIMES, DIVIDE,
    SEMI_COLON,
    LEFT_PAREN, RIGHT_PAREN,
    ID, NUM,
    ENDFILE, ERROR,WHITESPACE
} tokenType;


// Used for debugging only /////////////////////////////////////////////////////////
const char* TokenTypeStr[]=
        {
                "If", "Then", "Else", "End", "Repeat", "Until", "Read", "Write",
                "Assign", "Equal", "LessThan","GreatThan",
                "Plus", "Minus", "Times", "Divide",
                "SemiColon",
                "LeftParen", "RightParen",
                "ID", "Num",
                "EndFile", "Error" , "WhiteSpace"
        };


#endif //SCANNER_GLOBALS_H
