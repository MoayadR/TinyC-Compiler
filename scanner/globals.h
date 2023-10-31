//
// Created by shadow on 10/31/2023.
//

#ifndef SCANNER_GLOBALS_H
#define SCANNER_GLOBALS_H

typedef enum tokenType {
    EndOfFile ,
    InvalidToken ,
    WhiteSpace,
    InputToken , OutputToken ,
    SemiColumnToken , IFToken ,
    ThenToken , ElseToken ,
    EndToken , RepeatToken ,
    UntilToken , MULToken ,
    DIVToken , PlusToken ,
    SubToken , AssignToken ,
    LTToken , ETToken ,
    GTToken , OpenningBracketsToken,
    ClosingBracketsToken, Number,
    ID
} tokenType;

#endif //SCANNER_GLOBALS_H
