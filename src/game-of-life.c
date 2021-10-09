#include <ncurses.h>

#include "game.h"


int main()
{
    initscr();
    cbreak();
    nodelay(stdscr, true);
    noecho();
    curs_set(0);
    keypad(stdscr, true);

    Simulation_t sim;
    initSimulation(&sim, LINES - 1, COLS, 300000);
    simulate(&sim);

    freeSimulation(&sim);
    echo();
    nocbreak();
    endwin();
    return 0;
}