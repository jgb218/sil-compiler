/*
  CSE 109
  Jeff Bulick
  jgb218
  Program Description: Lexer class handles an input stream
  to create Tokens.
  Program #4
 */

#include <string>
#include <stdlib.h>
#include "lexer.h"
#include "token.h"

string lexeme;
int tokenType;
int line;
int pos;
int place;  //beginning index on line for the token
bool tokenComplete;

Lexer::Lexer(istream& in):in(in){
  line = 1;
  pos = 1;
}

Token* Lexer::nextToken(){
  lexeme = "";  //empty string to concatenate chars to form lexeme
  tokenComplete = false;  //tests whether token is ready to be initialized
  while(!tokenComplete){
    place = pos;
    char current = nextChar();  //get first char of token
    if(isalnum(current)){//handles group 1 tokens and keywords
      firstGroup(current);  //handles INTLIT and IDENT
      tokenComplete = true;
    }
    else if( (current == ' ') || (current == '\t') ){ //this ignores white space
     
    }
    else if(current == '\n'){  //adds counter for next line
      line++;
      pos = 1;  //position on line goes back to 1
    }
    else if(current == '#'){  //handles comments
      current = nextChar();
      while( (current != '#') && (in.peek() != -1) ){  //read characters
						 //until another # or
						 //end of file
	current = nextChar();
      }
    }
    else if(current == '$'){  //end of the file stream
      tokenType = Token::EOF;
      lexeme += "ENDOFFILE";
      tokenComplete = true;
    }
    else{  //char is in range of special characters
      specialChar(current);  //handles group 2 tokens (special characters)
      tokenComplete = true;  
    }
  }  
  Token* token = new Token(tokenType,lexeme,line,place);
  return token;
}

char Lexer::nextChar(){
  char ch;
  if(in.peek() != -1){  //checks for end of the file
    ch = in.get();  //gets next character from input stream
    pos++;  //move counter on line over
  }
  else{  //return dollar sign if end of the file
    return '$';  
  }
  return ch;
}

void Lexer::firstGroup(char ch){
  lexeme += ch;  //add first letter of token
  if(isalpha(ch)){  //starts with letter is an IDENT
    tokenType = Token::IDENT;
  }
  else if(isdigit(ch)){  //starts with number is an INTLIT
    tokenType = Token::INTLIT;
  }
  else{  
    tokenType = Token::ERROR;
  }
  while(isalnum(ch)){  //while character remains alphanumeric
    bool match = checkKeyword();  //check if token matches a keyword
    if(match){
      return;
    }
    ch = in.peek();  //check next char without removing from stream
    if(isalnum(ch)){
      lexeme += nextChar();
    }
  }
}

bool Lexer::checkKeyword(){
  bool check = false;
  if( lexeme == "set" ){
    tokenType = Token::SET;
    check = true;
  }
  else if(lexeme == "print"){
    tokenType = Token::PRINT;
    check = true;
  }
  else if(lexeme == "while"){
    tokenType = Token::WHILE;
    check = true;
  }
  else if(lexeme == "do"){
    tokenType = Token::DO;
    check = true;
  }
  else if(lexeme == "end"){  //handles difference between end/endif
    string temp = lexeme;
    temp += in.peek();
    if(temp == "endi"){
      lexeme += nextChar();
      temp += in.peek();  //look for "if" in next two chars
      if(temp == "endif"){
	tokenType = Token::ENDIF;
	lexeme += nextChar();
      }
      else{
	tokenType = Token::END;
      }
    }
    else{
      tokenType = Token::END;
    }
    check = true;
  }
  else if(lexeme == "and"){
    tokenType = Token::AND;
    check = true;
  }
  else if(lexeme == "or"){
    tokenType = Token::OR;
    check = true;
  }
  else if(lexeme == "if"){
    tokenType = Token::IF;
    check = true;
  }
  else if(lexeme == "then"){
    tokenType = Token::THEN;
    check = true;
  }
  else if(lexeme == "else"){
    tokenType = Token::ELSE;
    check = true;
  }
  else if(lexeme == "program"){
    tokenType = Token::PROGRAM;
    check = true;
  }
  else{
    check = false;
  }
  return check;
}

void Lexer::specialChar(char ch){
  string temp;
  switch(ch){
  case '+':
    tokenType = Token::PLUS;
    lexeme += ch;
    break;
  case '-':
    tokenType = Token::MINUS;
    lexeme += ch;
    break;
  case '*':
    tokenType = Token::TIMES;
    lexeme += ch;
    break;
  case '/':
    tokenType = Token::DIVIDE;
    lexeme += ch;
    break;
  case '%':
    tokenType = Token::REM;
    lexeme += ch;
    break;
  case '=':
    lexeme += ch;
    temp = lexeme;
    temp += in.peek();
    if(temp == "=="){ //equals operator
      tokenType = Token::EQ;
      lexeme += nextChar();
    }
    else{  //assignment operator
      tokenType = Token::ASSIGN;
    }
    break;
  case '(':
    tokenType = Token::LPAREN;
    lexeme += ch;
    break;
  case '{':
    tokenType = Token::LPAREN;
    lexeme += ch;
    break;
  case ')':
    tokenType = Token::RPAREN;
    lexeme += ch;
    break;
  case '}':
    tokenType = Token::RPAREN;
    lexeme += ch;
    break;
  case ',':
    tokenType = Token::COMMA;
    lexeme += ch;
    break;
  case '<':
    lexeme += ch;
    temp = lexeme;
    temp += in.peek();
    if(temp == "<="){   //<= operator
      tokenType = Token::LE;
      lexeme += nextChar();
    }
    else{  //< operator
      tokenType = Token::LT;
    }
    break;
  case '>':
    lexeme += ch;
    temp = lexeme;
    temp += in.peek();
    if(temp == ">="){
      tokenType = Token::GE;
      lexeme += nextChar();
    }
    else{
      tokenType = Token::GT;
    }
    break;
  case '!':
    lexeme += ch;
    temp = lexeme;
    temp += in.peek();
    if(temp == "!="){  //!= operator
      tokenType = Token::NE;
      lexeme += nextChar();
    }
    else{  //error
      tokenType = Token::ERROR;
    }
    break;
  default:
    tokenType = Token::ERROR;  //no lexeme matches this token
    lexeme += "ERROR";
  }
}
