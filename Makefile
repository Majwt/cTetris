# Makefile for Mac
SRCDIR=./source
CC=gcc-13
INCLUDE = /opt/homebrew/include/SDL2
INCDIR=./include

CFLAGS = -g -I$(INCLUDE) -I$(INCDIR) -c 
LDFLAGS = `sdl2-config --cflags --libs ` -lSDL2_image -lSDL2_ttf

Tetris: main.o tetrino.o board.o highscore.o utils.o
	$(CC) -o Tetris.out main.o tetrino.o board.o highscore.o utils.o $(LDFLAGS)

main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(SRCDIR)/main.c

tetrino.o: $(SRCDIR)/tetrino.c $(INCDIR)/tetrino.h  
	$(CC) $(CFLAGS) $(SRCDIR)/tetrino.c
board.o: $(SRCDIR)/board.c $(INCDIR)/board.h $(INCDIR)/tetrino.h  
	$(CC) $(CFLAGS) $(SRCDIR)/board.c
highscore.o: $(SRCDIR)/highscore.c $(INCDIR)/highscore.h 
	$(CC) $(CFLAGS) $(SRCDIR)/highscore.c
utils.o: $(SRCDIR)/utils.c $(INCDIR)/utils.h 
	$(CC) $(CFLAGS) $(SRCDIR)/utils.c

clean:
	rm *.out
	rm *.o