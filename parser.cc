/*
  CSE 109
  Jeff Bulick
  jgb218
  Program Description: The Parser class will check each token to make
  sure there are no syntax errors breaking language code and link
  nodes recursively to create a tree.
  Program 7
 */

#include "parser.h"
#include "token.h"
#include "lexer.h"
#include <cstring>

//global variables
int numVariables = 0;  //size of variable array 
string* variables = new string[100];  //array holds variables
int jNum = 1;  //increments the jumps in assembly

const string Parser::ops[] = 
   {"ADD", "SUB", "MULT", "DIV", "AND", "OR", "LIT", "VAR", 
     "STORE", "LABEL", "PRINT", "SEQ", "PRINTLN",
     "ISEQ", "ISLT", "ISLE", "ISGT", "ISGE", "ISNE", "JUMP", "JUMPF", "JUMPT" };

Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1) {
  nextToken();
}

Parser::~Parser() {

}

void Parser::error(string message) {
  cerr << "Error: " << message << ", Found '" << token.getLexeme() << "' at line " << token.getLine() << " position " << token.getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token.getType() != tokenType){
    error(message);
  }
}

void Parser::nextToken(){
  token = *lexer.nextToken(); 
}

//Post order recursion starting at top parent parameter
void Parser::postOrderVar(TreeNode* node){
  if(node != NULL){
    postOrderVar(node -> leftChild);
    postOrderVar(node -> rightChild);
    if( node -> op == STORE ){
      bool inArray = false;
      for(int i=0; i<=numVariables; i++){
	if(node->val==variables[i]){
	  inArray = true;
	}
      }
      if(!inArray){
	  variables[numVariables] = node->val;
	  numVariables++;
      }
    }
  }
}

//Post order traverse tree retrieve VARS
void Parser::vardefs(TreeNode* node){
  postOrderVar(node);
  printdefs();
}

//send variables to stdout
void Parser::printdefs(){
  for(int i=0; i<numVariables; i++){
    toasm(variables[i]+" resq 1");
  }
}

//Post order traversal of tree to generate assembly .text section
void Parser::postOrderInst(TreeNode* node){
  if(node != NULL){
    postOrderInst(node -> leftChild);
    postOrderInst(node -> rightChild);
    emit(node);
  }
}

//generates assembly code for each operation sends to stdout
void Parser::emit(TreeNode* node){
  switch(node -> op){
  case VAR:
    toasm("push qword["+node->val+"]");
    break;
  case LIT:
    toasm("mov rax,"+node->val);
    toasm("push rax");
    break;
  case ADD:
    toasm("pop rbx");
    toasm("pop rax");
    toasm("add rax,rbx");
    toasm("push rax");
    break;
  case SUB:
    toasm("pop rbx");
    toasm("pop rax");
    toasm("sub rax,rbx");
    toasm("push rax");
    break;
  case AND:
    toasm("pop rbx");
    toasm("pop rax");
    toasm("and rax,rbx");
    toasm("push rax");
    break;
  case OR:
    toasm("pop rbx");
    toasm("pop rax");
    toasm("or rax,rbx");
    toasm("push rax");
    break;
  case MULT:
    toasm("pop rbx");
    toasm("pop rax");
    toasm("imul rbx");
    toasm("push rax");
    break;
  case DIV:
    toasm("mov rdx,0");
    toasm("pop rbx");
    toasm("pop rax");
    toasm("idiv rbx");
    toasm("push rax");
    break;
  case STORE:
    toasm("pop qword["+node->val+"]");
    break;
  case LABEL:
    toasm(node->val);
    break;
  case ISEQ:
  case ISNE:
  case ISLT:
  case ISLE:
  case ISGT:
  case ISGE:
    handleRelExp(node->op);
    break;
  case JUMP:
    toasm("jmp "+node->val);
    break;
  case JUMPF:
    toasm("pop rax");
    toasm("cmp rax,0");
    toasm("je "+node->val);
    break;
  case JUMPT:
    toasm("pop rax");
    toasm("cmp rax,0");
    toasm("jne "+node->val);
    break;
  case PRINT:
    toasm("pop rax");
    toasm("push rbp");
    toasm("mov rdi,fmt");
    toasm("mov rsi,rax");
    toasm("mov rax,0");
    toasm("call printf");
    toasm("pop rbp");
    toasm("mov rax,0");
    break;
  case PRINTLN:
    toasm("push rbp");
    toasm("mov rdi,endl");
    toasm("mov rax,0");
    toasm("call printf");
    toasm("pop rbp");
    toasm("mov rax,0");
    break;  
  default: ;  //handles SEQ
  }
}

//handles assembly generation for relational expressions
void Parser::handleRelExp(Operation oper){
  toasm("pop rbx");
  toasm("pop rax");
  toasm("cmp rax,rbx");
  switch(oper){
  case ISEQ:
    toasm("je j"+itos(jNum));
    break;
  case ISNE:
    toasm("jne j"+itos(jNum));
    break;
  case ISLT:
    toasm("jl j"+itos(jNum));
    break;
  case ISLE:
    toasm("jle j"+itos(jNum));
    break;
  case ISGT:
    toasm("jg j"+itos(jNum));
    break;
  case ISGE:
    toasm("jge j"+itos(jNum));
    break;
  default:
    error("Incorrect operation for relational expression");
  }
  toasm("mov rax,0");
  toasm("jmp j"+itos(jNum+1));
  toasm("j"+itos(jNum)+":");
  toasm("mov rax,1");
  toasm("j"+itos(jNum+1)+":");
  toasm("push rax");

  jNum += 2;  //incrememnt jump number so assembly does not overlap
}

void Parser::geninst(TreeNode* node){
  postOrderInst(node);
}

//generate and send Intel Assembly code to stdout
void Parser::genasm(TreeNode* node){
  toasm("global main");
  toasm("extern printf");
  toasm("segment .bss");
  out << endl;
  vardefs(node);
  out << endl;
  toasm("section .data");
  toasm("fmt:   db '%ld ', 0");
  toasm("endl:   db 10, 0");
  toasm("section .text");
  toasm("main:");
  out << endl;
  geninst(node);
  out << endl;
  toasm("ret");  //end of assembly program
}

Parser::TreeNode* Parser::factor() {
  TreeNode* node = NULL; 
  if(token.getType()==Token::LPAREN){
    check(Token::LPAREN,"Factor starts with '('");
    nextToken();
    node = expression();
    check(Token::RPAREN,"Factor ends with ')'");
    nextToken();
  }
  else if(token.getType()==Token::INTLIT){
    node = new TreeNode( LIT,token.getLexeme() );
    nextToken();
  }
  else if(token.getType()==Token::IDENT){
    node = new TreeNode( VAR,token.getLexeme() ); 
    nextToken();
  }
  else{
    error("Factor should start with '(' or INTLIT or IDENT");
  }
  return node;
}

Parser::TreeNode* Parser::term() {
  TreeNode* node = NULL;
  node = factor();
    if(token.getType()==Token::TIMES){
      nextToken();
      return new TreeNode( MULT,"",node,term() );

    }
    else if(token.getType()==Token::DIVIDE){
      nextToken();
      return new TreeNode( DIV,"",node,term() );
    }
  return node;
}

Parser::TreeNode* Parser::expression() {
  TreeNode* node = NULL;

  node = term();
  while( (token.getType()==Token::PLUS)||(token.getType()==Token::MINUS) ){
    if(token.getType()==Token::PLUS){
      nextToken();
      return new TreeNode( ADD,"",node,expression() );
    }
    else if(token.getType()==Token::MINUS){
      nextToken();
      return new TreeNode( SUB,"",node,expression() );
    }
  }
  return node;
}

Parser::TreeNode* Parser::relationalExpression() {
  TreeNode* node = NULL;

  node = expression();
  if( (token.getType()==Token::EQ)||(token.getType()==Token::LT)||(token.getType()==Token::LE)||(token.getType()==Token::GT)||(token.getType()==Token::GE)||(token.getType()==Token::NE) ){

    switch(token.getType()){
    case Token::EQ:
      nextToken();
      return new TreeNode( ISEQ,"",node,relationalExpression() );
      break;
    case Token::LT:
      nextToken();
      return new TreeNode( ISLT,"",node,relationalExpression() );
      break;
    case Token::LE:
      nextToken();
      return new TreeNode( ISLE,"",node,relationalExpression() );
      break;
    case Token::GT:
      nextToken();
      return new TreeNode( ISGT,"",node,relationalExpression() );
      break;
    case Token::GE:
      nextToken();
      return new TreeNode( ISGE,"",node,relationalExpression() );
      break;
    case Token::NE:
      nextToken();
      return new TreeNode( ISNE,"",node,relationalExpression() );
      break;
    default:
      error("Not a relational expression");
    }
  }
  return node;
}

Parser::TreeNode* Parser::logicalExpression() {
  TreeNode* node = NULL;

  node = relationalExpression();
  while( (token.getType()==Token::AND)||(token.getType()==Token::OR) ){
    if(token.getType()==Token::AND){
      nextToken();
      return new TreeNode( AND,"",node,logicalExpression() ); 
    }
    else if(token.getType()==Token::OR){
      nextToken();
      return new TreeNode( OR,"",node,logicalExpression() );	
    }
  }
  return node;
}

Parser::TreeNode* Parser::setStatement() {
  check(Token::SET,"set statement begins with 'set'");
  nextToken();
  check(Token::IDENT,"An IDENT comes after 'set'");
  TreeNode* rightNode = new TreeNode( STORE,token.getLexeme() );
  nextToken();
  check(Token::ASSIGN,"Missing '='");
  nextToken();
  TreeNode* leftNode = expression();
  return new TreeNode( SEQ,"",leftNode,rightNode );
}

Parser::TreeNode* Parser::printStatement() {
  TreeNode* leftNode = NULL;
  TreeNode* rightNode = new TreeNode( PRINT );

  leftNode = expression();
  if(token.getType()==Token::COMMA){
    nextToken();
    return new TreeNode( SEQ,"",new TreeNode( SEQ,"",leftNode,rightNode ),printStatement() );
    //recursion accounts for loop to print more than one variable
  }
  else{
    return new TreeNode( SEQ,"",leftNode,rightNode);
  }
}

Parser::TreeNode* Parser::whileStatement() {
  string trueLabel = makeLabel();
  string falseLabel = makeLabel();
  TreeNode* rightNode = new TreeNode( SEQ,"",
      new TreeNode( JUMP, trueLabel ),new TreeNode( LABEL, falseLabel+":" )
  );  //jump and label is right node

  check(Token::WHILE,"while statement starts with 'while'");
  nextToken();
  TreeNode* lNode = new TreeNode( SEQ,"",new TreeNode( LABEL, trueLabel+":" ),logicalExpression() );
  check(Token::DO,"missing 'do' after logical expression");
  nextToken();
  TreeNode* rNode = new TreeNode( SEQ,"",new TreeNode( JUMPF, falseLabel ),compoundStatement() );
  check(Token::END,"missing 'end' of while statement");
  nextToken();
  return new TreeNode( SEQ,"",new TreeNode( SEQ,"",lNode,rNode ),rightNode );  
}

Parser::TreeNode* Parser::ifStatement() {
  string falseLabel = makeLabel();
  string trueLabel = makeLabel();
  TreeNode* node = NULL;
  TreeNode* lNode = NULL;
  TreeNode* ifNode = NULL;
  TreeNode* elseNode = NULL;
  TreeNode* compNode = NULL;
  
  check(Token::IF,"if statement begins with 'if'");
  nextToken();
  lNode = new TreeNode( SEQ,"",logicalExpression(),
			new TreeNode(JUMPF,falseLabel) );
  check(Token::THEN,"missing 'then' after logical expression");
  nextToken();
  compNode = compoundStatement();
  ifNode = new TreeNode( SEQ,"",lNode,compNode );
  node = new TreeNode( SEQ,"",ifNode,new TreeNode( LABEL,falseLabel+":" ) );

  if(token.getType()==Token::ELSE){
    TreeNode* rightIfNode = new TreeNode( SEQ,"",compNode,
					  new TreeNode(JUMP,trueLabel) );
    ifNode = new TreeNode( SEQ,"",lNode,rightIfNode );
    nextToken();
    elseNode = new TreeNode( SEQ,"",new TreeNode(LABEL,falseLabel+":"),
			     compoundStatement() );
    node = new TreeNode( SEQ,"",new TreeNode( SEQ,"",ifNode,elseNode ),
			 new TreeNode( LABEL,trueLabel+":" ) );
    check(Token::ENDIF,"missing 'endif'");
  }
  check(Token::ENDIF,"missing 'endif'");
  nextToken();
  return node;
}

Parser::TreeNode* Parser::statement() {
  TreeNode* node = NULL;
  if(token.getType() == Token::SET){
    node = setStatement();
  }
  else if(token.getType() == Token::PRINT){
    check(Token::PRINT,"print statement starts with 'print'");
    nextToken();
    node = new TreeNode( SEQ,"",printStatement(),new TreeNode(PRINTLN) );
  }
  else if(token.getType() == Token::WHILE){
    node = whileStatement();
  }
  else if(token.getType() == Token::IF){
    node = ifStatement();
  }
  else{}
  return node;
}

Parser::TreeNode* Parser::compoundStatement() {
  TreeNode* node = NULL;
  
  node = statement();
  if( (token.getType()==Token::SET)||(token.getType()==Token::PRINT)||(token.getType()==Token::WHILE)||(token.getType()==Token::IF) ){
    node = new TreeNode( SEQ,"",node,compoundStatement() );
  }
  return node;
}

Parser::TreeNode* Parser::program() {
  TreeNode* node = NULL;
  check(Token::PROGRAM,"Program should start with 'program'");
  nextToken();
  check(Token::IDENT,"Name of program cannot be a keyword");
  nextToken();
  node = compoundStatement();
  check(Token::END,"Second to last keyword should be 'end'");
  nextToken(); 
  check(Token::PROGRAM,"Last keyword should be 'program'");
  
  //out << "Program Accepted" << endl << endl;    //write to standard out if succesful
  return node;
}
