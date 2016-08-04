-----------------------------------------
/////////////////////////////////////////
    SIL Compiler into NASM Assembly
/////////////////////////////////////////
/////////////////////////////////////////
// Jeff Bulick
// Spring 2016
// This compiler creates and parses tokens to
// recursively generate a tree structure using C++, 
// and does a second pass through the tree to generate 
// Intel x86/NASM Assembly code.
/////////////////////////////////////////
-----------------------------------------

System Requirements:
- This is a Linux example 
- GCC/G++ compiler
- NASM compiler

To Compile and Run:
	1. Type in command line to create executable "silc"
		$ make
	2. Run executable and redirect stdout to create our Assembly file
		$ ./silc < **name of .sil file** > myprog.asm
	3. Compile Assembly and create linux binary
		./compile
	4. Run executable "myprog" to see stdout of .sil file
		$ ./myprog

List of Files:
	- silc.cc (main function)
	- parser.h, parser.cc
	- lexer.h, lexer.cc
	- token.h, token.cc
	- Makefile
	- compile (script for compiling Assembly x86)
	- README.txt
	(The made up imperative language example to be compiled)
	- test.sil

**Note: I did not create the logic for the SIL language, this was provided by professor.
