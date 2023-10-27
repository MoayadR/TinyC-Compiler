#include <iostream>
#include "scanner.h"
#include <fstream>
#include <sstream>

using namespace std;

Scanner::Scanner() {
    this->generateReservedWords();
    this->current = 0;
    this->position= 0;
}

Scanner::Scanner(const string &fileName) {
    this->setFileContent(fileName);
    this->generateReservedWords();
    this->generateListOfTokens();
    this->current = 0;
    this->position= 0;
}

void Scanner::generateListOfTokens() {
    while (true)
    {
        Token temp = this->getToken();
        this->listOfTokens.push_back(temp);
        if (temp.getType() == EndOfFile) // stopping Condition;
        {
            break;
        }
    }
}

Token Scanner::getToken() { // Depend on FileContent
    string temp = "";
    char c;
    while(this->current < this->fileContent.length())
    {
        c = this->fileContent[this->current];

        if(c == '{') // Skip Comments
        {
            while(c != '}')
            {
                this->current++;
                c = this->fileContent[this->current];
            }
            this->current++;
            c = this->fileContent[this->current];
        }

        while(isDigit(c)) // detect numbers
        {
            temp+=c;
            this->current++;
            c = this->fileContent[this->current];
            if(!isDigit(c))
            {
                return Token(temp , Number);
            }
        }

        if(c == ' ' && temp.length()) // detect white space and split using it
        {
            return Token(temp,ID);
        }

        if(c!=' ' && c!= '\n' && c!= '\t') // filling temp and looking ahead for delimiters
        {
            if(((this->delimiters.find(string(1, c)) != this->delimiters.end()) || c==':' )&& temp.length()) // if char (single OP) was a delimiter
            {
                // := case
                if(temp != ":" and c != '=')
                    return Token(temp , ID);
            }
            temp+=c;
        }

        this->current++; // increments current
        if(this->delimiters.find(temp) != this->delimiters.end()) // return delimiter as a token
        {
            return Token(temp , this->delimiters[temp]);
        }

    }

    if(this->current >= this->fileContent.length())
    {
        if (temp.length())
            return Token(temp,ID);

        return Token(" " , EndOfFile);
    }
}

void Scanner::printListOfTokens() {
    for (auto token : this->listOfTokens)
    {
        cout<<"Token Value: " << token.getValue() << " Token Type: " << token.getType()<<endl;
    }
}

void Scanner::generateReservedWords() {
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
}


void Scanner::setFileContent(const string &fileName) {
    fstream fileHandler(fileName);
    stringstream buffer;
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

bool isDigit(char c) {
    if(int(c) >= 48 && int(c) <= 57)
        return true;
    return false;
}


int main() {
    Scanner scanner("C:\\Users\\moaya\\OneDrive\\Desktop\\TinyC\\scanner\\scannerTest.txt");
    scanner.printListOfTokens();
    return 0;
}

