/*
  CSE 109
  Jeff Bulick
  jgb218
  Program Description: The Token is an object for the compiler
  classified by type, lexeme, and position in the document
  Program #4
*/

#include <string>
#include <iomanip>
#include "token.h"
using namespace std;

Token::Token(int type, string lexeme, int line, int pos):type(type),
     lexeme(lexeme),line(line),pos(pos){
}

Token::Token(){

}

Token::~Token(){

}

int Token::getType(){
  return type;
}

string Token::getLexeme(){
  return lexeme;
}

int Token::getLine(){
  return line;
}

int Token::getPos(){
  return pos;
}

void Token::printToken(){
  cout <<
    setw(8) << right << type <<
    setw(5) << "" <<
    setw(10) << left << lexeme <<
    setw(1) << "" <<
    setw(6) << right << line <<
    setw(2) << "" << 
    setw(6) << right << pos << endl;
}
