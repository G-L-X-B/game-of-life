EXE = game-of-life
CC = gcc
CFLAGS = -c -Wall -Wextra -Wpedantic
LDLIBS = -lncurses
OBJ = `find . -name "*.o"`

all: $(EXE)
$(EXE): game.o gameio.o life.o
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

life.o: life.c life.h
gameio.o: gameio.c gameio.h
game.o: game.c game.h life.h gameio.h
clean:
	rm -f $(EXE) $(OBJ)
