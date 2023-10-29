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
        this->updatePositionLine(c);

        if(c == '{') // Skip Comments
        {
            while(c != '}')
            {
                this->current++;
                c = this->fileContent[this->current];
                this->updatePositionLine(c);
            }
            this->current++;
            c = this->fileContent[this->current];
            this->updatePositionLine(c);
        }


        if(isspace(c))
        {
            this->current++;
            return Token(" " , WhiteSpace);
        }

        if(isdigit(c)) // detect numbers
        {
            while(isdigit(c))
            {
                temp += c;
                this->current++;
                c = this->fileContent[this->current];
            }
            return Token(temp , Number);
        }

        if(isalpha(c) || c == '_') // detect reservedWords or variables
        {
            while(isalnum(c))
            {
                temp += c;
                this->current++;
                c = this->fileContent[this->current];
            }
            if(this->delimiters.find(temp) != this->delimiters.end()) // checking if delimiter
                return Token(temp , this->delimiters[temp]);
            return Token(temp , ID);  // is ID then
        }

        if(ispunct(c))
        {
            while(ispunct(c))
            {
                temp += c;
                this->current++;

                if(this->delimiters.find(temp) != this->delimiters.end()) // checking if delimiter
                    return Token(temp , this->delimiters[temp]);

                c = this->fileContent[this->current];
            }

            cout<<"Error Invalid Token at " << this->position<<endl; // error handling el 8laba
            return Token(temp , InvalidToken); // then invalid punct
        }

        this->current++;
        cout<<"Error Invalid Token at " << this->position<<endl; // error handling el 8laba
        return Token(string(1,c) , InvalidToken);
    }

    if(this->current >= this->fileContent.length())
        return Token(" " , EndOfFile);
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

void Scanner::updatePositionLine(char c) {
    if(c == '\n')
    {
        this->position++;
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

