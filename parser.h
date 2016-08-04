/*
  CSE 109
  Jeff Bulick
  jgb218
  Program Description: Header file for the Parser class
  Program 7
 */

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sstream>

using namespace std;

class Parser {
  
private:
  Lexer lexer;
  Token token;
  ostream& out;
  
  int lindex;
  int tindex;

  enum Operation {
    ADD, SUB, MULT, DIV, AND, OR, LIT, VAR, 
    STORE, LABEL, PRINT, SEQ, PRINTLN,
    ISEQ, ISLT, ISLE, ISGT, ISGE, ISNE, JUMP, JUMPF, JUMPT
  };

  void error(string message);
  void check(int tokenType, string message);
  void nextToken();  //sets token member variable to next token
  
  string itos(int i) { stringstream ss; ss << i; string res = ss.str(); return res;}  
  string makeLabel() { string tmp = "L"; stringstream ss; ss << ++lindex; string res = ss.str(); tmp = tmp + res; return tmp;}
  //string makeTemp() { string tmp = "$TEMP"; stringstream ss; ss << ++tindex; string res = ss.str(); tmp = tmp + res; return tmp;}
  static const string ops[];  
  
 
public:
  class TreeNode {
	      
  public:

    Operation op;
    string val;
    TreeNode *leftChild;
    TreeNode *rightChild;

    void init(Operation opx, string valx, TreeNode *leftChildx, TreeNode *rightChildx) {
      op = opx;
      val = valx;
      leftChild = leftChildx;
      rightChild = rightChildx;
    }

    TreeNode(Operation op, string val) {
      init(op, val, NULL, NULL);
    }
    
    TreeNode(Operation op, string val, TreeNode *leftChild, TreeNode *rightChild) {
      init(op, val, leftChild, rightChild);
    }

    TreeNode(Operation op) {
      init(op, "", NULL, NULL);
    }

    TreeNode(Operation op, TreeNode *leftChild, TreeNode *rightChild) {
      init(op, "", leftChild, rightChild);
    }

    static string toString(TreeNode *node) {
      return toString0(node, 0);
    }
    
    static string toString0(TreeNode *node, int spaces) {
      static string blanks = "                                        ";
      string left = "";
      string right = "";
      bool isLeaf = true;
      if (node->leftChild != NULL) {
	left = toString0(node->leftChild, spaces+2);
	isLeaf = false;
      }
      if (node->rightChild != NULL) {
	right = toString0(node->rightChild, spaces+2);
	isLeaf = false;	
      }
      string ret;
      if (isLeaf) {
	ret = blanks.substr(0, spaces) + ops[node->op] + "[" + node->val + "]";
      } else {
	ret = blanks.substr(0, spaces) + ops[node->op] + "(\n" + left + ",\n" + right + "\n" + 
	      blanks.substr(0, spaces) + ")";
      }
      return ret;
    }
  };

  TreeNode* program();
  TreeNode* compoundStatement();
  TreeNode* statement();
  TreeNode* setStatement();
  TreeNode* printStatement();
  TreeNode* whileStatement();
  TreeNode* ifStatement();
  TreeNode* logicalExpression();
  TreeNode* relationalExpression();
  TreeNode* expression();
  TreeNode* term();
  TreeNode* factor();

public:
  Parser(Lexer& lexer, ostream& out);
  ~Parser();
  void genasm(TreeNode* node);
private:
  void vardefs(TreeNode* node);
  void printdefs();
  void postOrderVar(TreeNode* node);
  void geninst(TreeNode* node);
  void postOrderInst(TreeNode* node);
  void emit(TreeNode* node);
  void handleRelExp(Operation oper);
  void toasm(string send){  //send string to standard out
    out << "\t" << send << endl;
  }
};

#endif
