# TinyC Compiler
## Overview

TinyC is a simple compiler that translates a small subset of the C programming language into intermediate code. This compiler consists of a scanner and a parser written in C++. The grammar defines the syntax of the language, and the scanner identifies tokens in the source code, which are then processed by the parser.

## Grammer
```EBNF
program -> stmtseq
stmtseq -> stmt { ; stmt }
stmt -> ifstmt | repeatstmt | assignstmt | readstmt | writestmt
ifstmt -> if exp then stmtseq [ else stmtseq ] end
repeatstmt -> repeat stmtseq until expr
assignstmt -> identifier := expr
readstmt -> read identifier
writestmt -> write expr
expr -> mathexpr [ (<|=) mathexpr ]
mathexpr -> term { (+|-) term }
term -> factor { (*|/) factor }
factor -> newexpr { ^ newexpr }
newexpr -> ( mathexpr ) | number | identifier
```

## Code Generation
Code generation was simulated in c++ for simplicity.

## Usage
* Clone the repository: git clone https://github.com/MoayadR/TinyC-Compiler
* Configure the cmake file with the header and cpp files 
* Write the TinyC program in the input.txt then run main.cpp

## Example for a TinyC Program
```tinyc
{ Sample program
  in TINY language
  compute factorial
}

read x; {input an integer}
if 0<x then {compute only if x>=1}
  fact:=1;
  repeat
    fact := fact * x;
    x:=x-1
  until x=0;
  write fact {output factorial}
end
```

## Contributions
Contributions are welcome! If you find any bugs or want to add new features, please open an issue or create a pull request.

## License
This project is licensed under the MIT License - see the LICENSE file for details.
