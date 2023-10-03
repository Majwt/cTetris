# Makefile for Mac
SRCDIR=./source
CC=gcc-13
INCLUDE = `sdl2-config --prefix`/include/SDL2
INCDIR=./include

CFLAGS = -g -I$(INCLUDE) -I$(INCDIR) -c 
LDFLAGS = `sdl2-config --cflags --libs ` -lSDL2_image -lSDL2_ttf

Tetris: main.o tetromino.o board.o highscore.o utils.o special.o
	$(CC) -o Tetris.out main.o tetromino.o board.o highscore.o utils.o special.o $(LDFLAGS)

main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(SRCDIR)/main.c

tetromino.o: $(SRCDIR)/tetromino.c $(INCDIR)/tetromino.h
	$(CC) $(CFLAGS) $(SRCDIR)/tetromino.c
board.o: $(SRCDIR)/board.c $(INCDIR)/board.h $(INCDIR)/tetromino.h  
	$(CC) $(CFLAGS) $(SRCDIR)/board.c
highscore.o: $(SRCDIR)/highscore.c $(INCDIR)/highscore.h 
	$(CC) $(CFLAGS) $(SRCDIR)/highscore.c
utils.o: $(SRCDIR)/utils.c $(INCDIR)/utils.h 
	$(CC) $(CFLAGS) $(SRCDIR)/utils.c
special.o: $(SRCDIR)/special.c $(INCDIR)/special.h 
	$(CC) $(CFLAGS) $(SRCDIR)/special.c

run: Tetris
	./Tetris.out

clean:
	rm *.out
	rm *.o
cleanrun: clean Tetris
	./Tetris.out
cleanbuild: clean Tetris