#include <ncurses.h>

#include "gameio.h"
#include "game.h"
#include "logger.h"


int main()
{
    initscr();
    cbreak();
    nodelay(stdscr, true);
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    FILE *f = fopen("log.txt", "w");
    lg_set_level(LOG_DEBUG);
    lg_add_fp(f, LOG_TRACE);

    Game_t sim;
    OutputQueue_t q;
    io_initOutputQueue(&q, (CharMap_t){'#', ' ', '?'}, A_REVERSE, (point2d_t){0, 0});

    initGame(&sim, LINES - 1, COLS, 400000);
    simulate(&sim, &q);

    freeGame(&sim);

    fflush(f);
    fclose(f);

    echo();
    nocbreak();
    endwin();
    return 0;
}
