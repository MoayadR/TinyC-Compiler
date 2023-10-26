//
// Created by moaya on 10/26/2023.
//

#ifndef SCANNER_SCANNER_H
#define SCANNER_SCANNER_H
#define LOOKAHEAD_BUFFER 6
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

typedef enum tokenType {
 EndOfFile ,
 InvalidToken ,
 InputToken , OutputToken ,
 SemiColumnToken , IFToken ,
 ThenToken , ElseToken ,
 EndToken , RepeatToken ,
 UntilToken , MULToken ,
 DIVToken , PlusToken ,
 SubToken , AssignToken ,
 LTToken , ETToken ,
 GTToken , OpenningBracketsToken,
 ClosingBracketsToken , OpenningCurlyBracketsToken,
 ClosingCurlyBracketsToken
} tokenType;

class Token{
private:
    string value;
    tokenType type;
public:
    Token();
    Token(const string &value, tokenType type);

    void setValue(const string &value);

    void setType(tokenType type);

    const string &getValue() const;
    tokenType getType() const;


};


class Scanner{
private:
    vector<Token>listOfTokens;
    unordered_map<string, tokenType>delimiters;
    string fileContent;
    int position; // error handling

public:
    Scanner();
    Scanner(const string& fileName);
    Token getToken();
    void generateListOfTokens();
    tokenType getTokenType(const string value);
    void printListOfTokens();
    void generateReserverWords();
    void setFileContent(const string& fileName);
    bool lookAheadForDelimiters();
};


#endif //SCANNER_SCANNER_H
