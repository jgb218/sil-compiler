/*
  CSE 109
  Jeff Bulick
  jgb218
  Program Description: Header file for the Token Class
  Program #4
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
using namespace std;

class Token{

private:
  //member variables
  int type; //the integer corresponding to the type of token
  string lexeme;  // the lexeme
  int line;  //the line number the token was found on
  int pos;  //character position on the line where token was found

public:
  //constructor
  Token(int type, string lexeme, int line, int pos);
  //default constructor
  Token();
  //destructor
  ~Token();
  //functions that return the values of private member variables
  int getType();
  string getLexeme();
  int getLine();
  int getPos();
  //print function
  void printToken();  //test function that prints in format for Prog4
  //these variables represent the token types
  static const int INTLIT = 1, IDENT = 2, PLUS = 3, MINUS = 4, TIMES = 5, DIVIDE = 6, REM = 7, ASSIGN = 8, LPAREN = 9, RPAREN = 10, COMMA = 11, EQ = 12, LT = 13, LE = 14, GT = 15, GE = 16, NE = 17, SET = 18, PRINT = 19, WHILE = 20, DO = 21, END = 22, AND = 23, OR = 24, IF = 25, THEN = 26, ELSE = 27, ENDIF = 28, PROGRAM = 29, EOF = 30, ERROR = 31;
  
};

#endif
