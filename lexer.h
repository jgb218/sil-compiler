/*
  CSE 109
  Jeff Bulick
  jgb218
  Program Description: Header file for Lexer class
  Program #4
 */

#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include "token.h"
using namespace std;

class Lexer {

private:
  //member variables
  istream& in;  //a reference to the passed in istream&
  char nextChar();  //reads value of next char and returns it
  void specialChar(char ch);  //handles case where lexeme is special char
  void firstGroup(char ch);  //handles first group of tokens
  bool checkKeyword();  //handles keyword group
public:
  //constructor
  Lexer(istream& in);
  //member functions
  Token* nextToken();  //returns the next Token from input stream
};

#endif
