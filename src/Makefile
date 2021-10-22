EXE = game-of-life
CC = gcc
CFLAGS = -c -Wall -Wextra -Wpedantic
LDLIBS = -lncurses
OBJ = `find . -name "*.o"`

all: $(EXE)
$(EXE): game-of-life.o game.o gameio.o life.o
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

game-of-life.o: game-of-life.c game.h
game.o: game.c game.h life.h gameio.h point2d.h
gameio.o: gameio.c gameio.h game.h point2d.h
life.o: life.c life.h point2d.h

clean:
	rm -f $(EXE) $(OBJ)
