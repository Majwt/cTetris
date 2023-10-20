# Makefile for Mac
SRCDIR=./source
CC=gcc-13
INCLUDE = `sdl2-config --prefix`/include/SDL2
INCDIR=./include

CFLAGS = -g -I$(INCLUDE) -I$(INCDIR) -c 
LDFLAGS = `sdl2-config --cflags --libs ` -lSDL2_image -lSDL2_ttf

Tetris: main.o game.o tetromino.o board.o highscore.o text.o common.o
	$(CC) -o Tetris.out main.o game.o tetromino.o board.o highscore.o text.o common.o $(LDFLAGS)

main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(SRCDIR)/main.c

game.o: $(SRCDIR)/game.c $(INCDIR)/game.h
	$(CC) $(CFLAGS) $(SRCDIR)/game.c
tetromino.o: $(SRCDIR)/tetromino.c $(INCDIR)/tetromino.h
	$(CC) $(CFLAGS) $(SRCDIR)/tetromino.c
board.o: $(SRCDIR)/board.c $(INCDIR)/board.h $(INCDIR)/tetromino.h  
	$(CC) $(CFLAGS) $(SRCDIR)/board.c
highscore.o: $(SRCDIR)/highscore.c $(INCDIR)/highscore.h 
	$(CC) $(CFLAGS) $(SRCDIR)/highscore.c
text.o: $(SRCDIR)/text.c $(INCDIR)/text.h 
	$(CC) $(CFLAGS) $(SRCDIR)/text.c
common.o: $(SRCDIR)/common.c $(INCDIR)/common.h 
	$(CC) $(CFLAGS) $(SRCDIR)/common.c

run: Tetris
	./Tetris.out
build: Tetris
clean:
	rm *.out
	rm *.o
cleanrun: clean Tetris
	./Tetris.out
cleanbuild: clean Tetris