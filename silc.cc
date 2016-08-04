/*
  CSE 109
  Jeff Bulick
  jgb218
  Program Description: Test main method to run a Parser
  and generate a tree.
  Program 6
 */

#include <iostream>
#include <string>
#include "token.h"
#include "lexer.h"
#include "parser.h"

using namespace std;

int main(int argc, char** argv){

  Lexer* lexer = new Lexer(cin);
  Parser* parser = new Parser(*lexer, cout);
  Parser::TreeNode* p = parser -> program();
  
  //cout << Parser::TreeNode::toString(p) << endl;

  parser -> genasm(p);
  
  return 0;
}
