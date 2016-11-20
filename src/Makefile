# Makefile

CC = g++
CFLAGS = -g -Wall -ansi -pedantic

PGM = gdg

$(PGM): $(PGM).tab.o $(PGM).lex.o main.o
	$(CC) $(CFLAGS) $(PGM).tab.o $(PGM).lex.o main.o -o $@ -lfl

$(PGM).tab.o: $(PGM).tab.c $(PGM).tab.h heading.h
	$(CC) $(CFLAGS) -c $(PGM).tab.c -o $@

$(PGM).tab.c: $(PGM).y heading.h
	bison -d -v $(PGM).y

$(PGM).lex.o: $(PGM).lex.c $(PGM).tab.h heading.h
	$(CC) $(CFLAGS) -c $(PGM).lex.c -o $@

$(PGM).lex.c: $(PGM).lex $(PGM).tab.h heading.h
	flex -o $@ $(PGM).lex

main.o: main.cpp heading.h
	$(CC) $(CFLAGS) -c main.cpp -o $@

clean:
	rm -Rf gdg *.o *~ *.lex.c *.tab.* *.output
