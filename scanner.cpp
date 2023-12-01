#include <iostream>
#include <fstream>
#include <sstream>
#include "scanner.h"

// Used for debugging only /////////////////////////////////////////////////////////




Scanner::Scanner() {
    this->generateReservedWords();
    this->current = 0;
    this->position= 1;
}

Scanner::Scanner(const string &fileName) {
    this->setFileContent(fileName);
    this->generateReservedWords();
//    this->generateListOfTokens();
    this->current = 0;
    this->position= 1;
}

void Scanner::generateListOfTokens() {
    while (true)
    {
        Token temp = this->getToken();
        this->listOfTokens.push_back(temp);
        if (temp.getType() == ENDFILE) // stopping Condition;
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
                this->updatePositionLine(c);
            } // c = }
            this->current++;
            c = this->fileContent[this->current];
        }

        if(c == '\n')
        {
            this->updatePositionLine(c);
            this->current++;
            continue;
        }
        if(isspace(c))
        {
            this->current++;
            continue;
        }

        if(isdigit(c)) // detect numbers
        {
            while(isdigit(c))
            {
                temp += c;
                this->current++;
                c = this->fileContent[this->current];
            }
            return Token(temp , NUM , this->position);
        }

        if(isalpha(c) || c == '_') // detect reservedWords or variables
        {
            while(isalpha(c) || c == '_')
            {
                temp += c;
                this->current++;
                c = this->fileContent[this->current];
            }
            if(this->delimiters.find(temp) != this->delimiters.end()) // checking if delimiter
                return Token(temp , this->delimiters[temp] , this->position);
            return Token(temp , ID , this->position);  // is ID then
        }

        if(ispunct(c))
        {
            while(ispunct(c))
            {
                temp += c;
                this->current++;

                if(this->delimiters.find(temp) != this->delimiters.end()) // checking if delimiter
                    return Token(temp , this->delimiters[temp] , this->position);

                c = this->fileContent[this->current];
            }

            return Token(temp , ERROR , this->position); // then invalid punct
        }

        this->current++;
        return Token(string(1,c) , ERROR , this->position);
    }

    if(this->current >= this->fileContent.length())
        return Token(" " , ENDFILE ,this->position);
    return Token(" " , ENDFILE ,this->position);
}

void Scanner::outputListOfTokens() {
    fstream output("..\\output.txt" , ios::out);

    for (const auto& token : this->listOfTokens)
    {
        output<<"[ "<<token.getLine()<<" ] "<< token.getValue() << " ( " << TokenTypeStr[token.getType()] << " )"<<'\n';
    }
    output.close();
}

void Scanner::generateReservedWords() {
    this->delimiters["read"] = READ;
    this->delimiters["write"] = WRITE;
    this->delimiters[";"] = SEMI_COLON;
    this->delimiters["if"] = IF;
    this->delimiters["then"] = THEN;
    this->delimiters["else"] = ELSE;
    this->delimiters["end"] = END;
    this->delimiters["repeat"] = REPEAT;
    this->delimiters["until"] = UNTIL;
    this->delimiters["*"] = TIMES;
    this->delimiters["/"] = DIVIDE;
    this->delimiters["+"] = PLUS;
    this->delimiters["-"] = MINUS;
    this->delimiters[":="] = ASSIGN;
    this->delimiters["<"] = LESS_THAN;
    this->delimiters["="] = EQUAL;
    this->delimiters[">"] = GREAT_THAN;
    this->delimiters["("] = LEFT_PAREN;
    this->delimiters[")"] = RIGHT_PAREN;
    this->delimiters["^"] = POWER;

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

Token Scanner::getWithoutConsumtion() {
    int tempCurrent = this->current;
    int tempPos = this->position;
    Token token = getToken();
    this->current = tempCurrent;
    this->position = tempPos;
    return token;
}


Token::Token(const string &value, tokenType type , int line) : value(value), type(type) , line(line) {}

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

int Token::getLine() const {
    return line;
}



