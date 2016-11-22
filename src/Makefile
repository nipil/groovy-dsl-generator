# Makefile

CC = g++
CFLAGS = -g -Wall -ansi -pedantic

PGM = gdg

$(PGM): $(PGM).tab.o $(PGM).lex.o myparser.o dslgen.o main.o
	$(CC) $(CFLAGS) $(PGM).tab.o $(PGM).lex.o myparser.o dslgen.o main.o -o $@ -lfl

$(PGM).tab.o: $(PGM).tab.c $(PGM).tab.h myparser.h heading.h
	$(CC) $(CFLAGS) -c $(PGM).tab.c -o $@

$(PGM).tab.c: $(PGM).y myparser.h heading.h
	bison -d -v $(PGM).y

$(PGM).lex.o: $(PGM).lex.c $(PGM).tab.h myparser.h heading.h
	$(CC) $(CFLAGS) -c $(PGM).lex.c -o $@

$(PGM).lex.c: $(PGM).lex $(PGM).tab.h myparser.h heading.h
	flex -o $@ $(PGM).lex

main.o: main.cpp myparser.h dslgen.h heading.h
	$(CC) $(CFLAGS) -c main.cpp -o $@

myparser.o: myparser.cpp myparser.h heading.h
	$(CC) $(CFLAGS) -c myparser.cpp -o $@

dslgen.o: dslgen.cpp dslgen.h myparser.h heading.h
	$(CC) $(CFLAGS) -c dslgen.cpp -o $@

clean:
	rm -Rf gdg *.o *~ *.lex.c *.tab.* *.output
