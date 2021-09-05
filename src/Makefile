EXE = game-of-life
CC = gcc
CFLAGS = -c -Wall -Wextra -Wpedantic
LDLIBS = -lncurses
OBJ = `find . -name "*.o"`

all: $(EXE)
$(EXE): game.o life.o
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

life.o: life.c life.h
game.o: game.c life.h
clean:
	rm -f $(EXE) $(OBJ)
