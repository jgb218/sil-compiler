OPTS = -g -c -Wall -Werror
objects = lexer.o token.o silc.o parser.o
silc: $(objects)
	g++ -o silc $(objects)
silc.o: lexer.o token.o silc.cc
	g++ $(OPTS) silc.cc
parser.o: parser.h parser.cc
	g++ $(OPTS) parser.cc
lexer.o: lexer.h lexer.cc
	g++ $(OPTS) lexer.cc
token.o: token.h token.cc
	g++ $(OPTS) token.cc
clean: 
	rm -f *.o *~ *.asm myprog silc
