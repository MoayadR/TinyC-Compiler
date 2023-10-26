#include <iostream>
#include "scanner.h"

using namespace std;

Scanner::Scanner() {}

Scanner::Scanner(const string &fileName) {
    this->fileHandler.open(fileName);
    this->position = 0;
    this->generateListOfTokens();
}

void Scanner::generateListOfTokens() {
    while (true)
    {
        Token temp = this->getTokenOnWhiteSpace();
        if (temp.getType() == EndOfFile) // stopping Condition;
        {
            break;
        }
        this->listOfTokens.push_back(temp);
    }
}

Token Scanner::getToken() {

}

Token Scanner::getTokenOnWhiteSpace() {
    string temp = "";
    char c;
    while(this->fileHandler >> noskipws >> c)
    {
        if (c == ' ' && temp.length() == 0)
            continue;
        else if (c == ' ')
            return Token(temp , this->getTokenType(temp));

        temp += c;
    }

    if (temp.length())
        return Token(temp , this->getTokenType(temp));

    this->fileHandler.close();
    return Token("" , EndOfFile);
}

tokenType Scanner::getTokenType(const string value) { // detect the type of the token
    return InvalidToken;
}

void Scanner::printListOfTokens() {
    for (auto token : this->listOfTokens)
    {
        cout<<"Token Value: " << token.getValue() << " Token Type: " << token.getType()<<endl;
    }
}



Token::Token(const string &value, tokenType type) : value(value), type(type) {}

Token::Token() {}

const string &Token::getValue() const {
    return value;
}

tokenType Token::getType() const {
    return type;
}

void Token::setValue(const string &value) {
    this->value = value;
}

void Token::setType(tokenType type) {
    this->type = type;
}


int main() {
    Scanner scanner("C:\\Users\\moaya\\OneDrive\\Desktop\\TinyC\\scanner\\scannerTest.txt");
    scanner.printListOfTokens();
    return 0;
}