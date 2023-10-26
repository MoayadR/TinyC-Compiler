#include <iostream>
#include "scanner.h"
#include <sstream>

using namespace std;

Scanner::Scanner() {
    this->generateReserverWords();
}

Scanner::Scanner(const string &fileName) {
    this->setFileContent(fileName);
    this->generateReserverWords();
    this->generateListOfTokens();
}

void Scanner::generateListOfTokens() {
    while (true)
    {
        Token temp = this->getToken();
        if (temp.getType() == EndOfFile) // stopping Condition;
        {
            break;
        }
        this->listOfTokens.push_back(temp);
    }
}

Token Scanner::getToken() { // Depend on FileContent
    string temp = "";
    char c;
    while(fileHandler >> noskipws >> c)
    {
        if (c == '{') // handling comments in TinyC
        {
            while(fileHandler>>noskipws>>c)
            {
                if (c== '}')
                {
                    fileHandler>>noskipws>>c;
                    break;
                }
            }
        }

        if (c == ' ' && temp.length() == 0)
            continue;
        else if (c == ' ')
            return Token(temp , this->getTokenType(temp));

        if(this->lookAheadForDelimiters() && temp.length())
            return Token(temp , this->getTokenType(temp));

        temp += c;
    }
    if (temp.length())
        return Token(temp , this->getTokenType(temp));

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

void Scanner::generateReserverWords() {
    this->delimiters["read"] = InputToken;
    this->delimiters["write"] = OutputToken;
    this->delimiters[";"] = SemiColumnToken;
    this->delimiters["if"] = IFToken;
    this->delimiters["then"] = ThenToken;
    this->delimiters["else"] = ElseToken;
    this->delimiters["end"] = EndToken;
    this->delimiters["repeat"] = RepeatToken;
    this->delimiters["until"] = UntilToken;
    this->delimiters["*"] = MULToken;
    this->delimiters["/"] = DIVToken;
    this->delimiters["+"] = PlusToken;
    this->delimiters["-"] = SubToken;
    this->delimiters[":="] = AssignToken;
    this->delimiters["<"] = LTToken;
    this->delimiters["="] = ETToken;
    this->delimiters[">"] = GTToken;
    this->delimiters["("] = OpenningBracketsToken;
    this->delimiters[")"] = ClosingBracketsToken;
    this->delimiters["{"] = OpenningCurlyBracketsToken;
    this->delimiters["}"] = ClosingCurlyBracketsToken;
}

bool Scanner::lookAheadForDelimiters() {
    string temp = "";
    char c ;
    for(int i =0 ; i<6 ; i++)
    {

    }
}

void Scanner::setFileContent(const string &fileName) {
    fstream fileHandler(fileName);
    std::stringstream buffer;
    buffer << fileHandler.rdbuf();
    this->fileContent =  buffer.str();
    fileHandler.close();
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