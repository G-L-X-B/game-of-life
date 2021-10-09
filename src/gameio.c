#include "gameio.h"

#include <ncurses.h>


CharMap_t CharMap = {'#', ' ', '?'};


char cellChar(const Cell_t cell)
{
    int ch;
    switch (cell.state) {
    case CS_ALIVE:
        ch = CharMap.alive;
        break;
    case CS_DEAD:
        ch = CharMap.dead;
        break;
    default:
        ch = CharMap.unknown;
    }
    return ch;
}


int printStatusBar(const Simulation_t *s)
{
    mvprintw(0, 0, "Step: %4llu", s->step);
    printw("  Alive: %5u", s->cur->alive);
    printw("  Max: %5u", s->max_alive);
    printw("  Delay: %5ums", s->iter_delay / 1000);
    return 0;
}

int printField(const Field_t *f)
{
    for (unsigned y = 0; y < f->height; ++y) {
        move(y + 1, 0);
        for (unsigned x = 0; x < f->width; ++x)
            addch(cellChar(f->field[y][x]));
    }
    return 0;
}

int highlightCell(const Simulation_t *sim, uint32_t y, uint32_t x)
{
    if (y >= (uint32_t)LINES - 1 || x >= (uint32_t)COLS)
        return -1;
    mvaddch(y + 1, x, cellChar(sim->cur->field[y][x]) | A_REVERSE);
    return 0;
}

int unhighlightCell(const Simulation_t *sim, uint32_t y, uint32_t x)
{
    if (y >= (uint32_t)LINES - 1 || x >= (uint32_t)COLS)
        return -1;
    mvaddch(y + 1, x, cellChar(sim->cur->field[y][x]) | A_NORMAL);
    return 0;
}


InputEvent_t getinput()
{
    InputEvent_t e;
    switch (e.input = getch()) {
    case ERR:
        e.type = IET_NONE;
        break;
    case 's':
        e.type = IET_PAUSE;
        break;
    case 'q':
        e.type = IET_STOP;
        break;
    case 'c':
        e.type = IET_CONTINUE;
        break;
    case '+':
        e.type = IET_SPEED_UP;
        break;
    case '-':
        e.type = IET_SLOW_DOWN;
        break;
    case KEY_UP:
        e.type = IET_UP;
        break;
    case KEY_DOWN:
        e.type = IET_DOWN;
        break;
    case KEY_LEFT:
        e.type = IET_LEFT;
        break;
    case KEY_RIGHT:
        e.type = IET_RIGHT;
        break;
    case ' ':
        e.type = IET_SWITCH;
        break;
    default:
        e.type = IET_OTHER;
        break;
    }
    return e;
}
