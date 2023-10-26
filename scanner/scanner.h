//
// Created by moaya on 10/26/2023.
//

#ifndef SCANNER_SCANNER_H
#define SCANNER_SCANNER_H
#include <fstream>
#include <vector>

using namespace std;

typedef enum tokenType {
 EndOfFile , InvalidToken
}tokenType;

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
    fstream fileHandler;
    vector<Token>listOfTokens;
    int position ; // for error handling

public:
    Scanner();
    Scanner(const string& fileName);
    Token getToken();
    Token getTokenOnWhiteSpace();
    void generateListOfTokens();
    tokenType getTokenType(const string value);
    void printListOfTokens();
};


#endif //SCANNER_SCANNER_H
