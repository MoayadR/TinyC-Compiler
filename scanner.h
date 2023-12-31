
#ifndef TINYC_COMPILER_SCANNER_H
#define TINYC_COMPILER_SCANNER_H
#include "globals.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Token{
private:
    string value;
    tokenType type;
    int line;
public:
    Token();
    Token(const string &value, tokenType type , int line);

    void setValue(const string &value);

    void setType(tokenType type);

    const string &getValue() const;
    tokenType getType() const;

    int getLine() const;


};


class Scanner{
private:
    vector<Token>listOfTokens;
    unordered_map<string, tokenType>delimiters;
    string fileContent;
    int position = 1;// error handling
    int current = 0;

public:
    Scanner();
    Scanner(const string& fileName);
    Token getToken();
    Token getWithoutConsumtion();
    void generateListOfTokens();
    void outputListOfTokens();
    void generateReservedWords();
    void setFileContent(const string& fileName);
    void updatePositionLine(char c);
};


#endif //TINYC_COMPILER_SCANNER_H
