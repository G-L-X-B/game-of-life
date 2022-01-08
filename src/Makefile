EXE = game-of-life
CC = gcc
CFLAGS = -c -Wall -Wextra -Wpedantic
LDLIBS = -lncurses
OBJ = `find . -name "*.o"`

all: $(EXE)

$(EXE): game-of-life.o game.o gameio.o gamei.o life.o point2d.o log.o
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

game-of-life.o: game-of-life.c game.h
game.o: game.c game.h life.h gameio.h point2d.h
gameio.o: gameio.c gameio.h gamei.h point2d.h
gamei.o: gamei.c gamei.h
life.o: life.c life.h point2d.h
point2d.o: point2d.c point2d.h
log.o: log.c/src/log.c log.c/src/log.h
	$(CC) $(CFLAGS) -c -o log.o log.c/src/log.c -DLOG_USE_COLOR

clean:
	rm -f $(EXE) $(OBJ)
