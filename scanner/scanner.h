//
// Created by moaya on 10/26/2023.
//

#ifndef SCANNER_SCANNER_H
#define SCANNER_SCANNER_H
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
 ClosingBracketsToken, Number,
 ID
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
    int current = 0;

public:
    Scanner();
    Scanner(const string& fileName);
    Token getToken();
    void generateListOfTokens();
    void printListOfTokens();
    void generateReservedWords();
    void setFileContent(const string& fileName);
};



#endif //SCANNER_SCANNER_H
