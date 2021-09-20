#include <time.h>
#include <unistd.h>

#include <ncurses.h>

#include "life.h"

int printfield(const Field_t *f, char blank)
{
    move(1, 0);
    for (unsigned y = 0; y < f->height; ++y) {
        for (unsigned x = 0; x < f->width; ++x)
            addch(f->field[y][x].state == CS_ALIVE ? '#' : blank);
        move(y + 1, 0);
    }
    return 0;
}

unsigned delay = 400000;

int main()
{
    initscr();
    cbreak();
    nodelay(stdscr, true);
    noecho();
    curs_set(0);

    Field_t prev, next;
    initField(&prev, LINES - 1, COLS);
    initField(&next, LINES - 1, COLS);
    Cell_t **f = prev.field;
    f[10][10].state = CS_ALIVE;
    f[11][10].state = CS_ALIVE;
    f[9][10].state = CS_ALIVE;
    f[11][11].state = CS_ALIVE;
    f[10][9].state = CS_ALIVE;

    int input;
    uint64_t step = 0;
    prev.alive = 5;
    uint32_t max = prev.alive;
    int before_next_frame = 0;
    while (1) {
        if ((input = getch()) != ERR) {
            if (input == '+') {
                delay = delay < 50000 ? 0 : delay - 50000;
                before_next_frame -= 50000;
            } else if (input == '-') {
                delay += 50000;
                before_next_frame += 50000;
            } else {
                nodelay(stdscr, false);
                input = getch();
                if (input == 's')
                    break;
                nodelay(stdscr, true);
            }
        }
        if (before_next_frame <= 0) {
            printfield(&prev, ' ');
            refresh();
            before_next_frame = delay;
            iterate(&prev, &next);
            Field_t f = prev;
            prev = next;
            next = f;
            if (prev.alive > max)
                max = prev.alive;
            ++step;
        }
        usleep(1000);
        before_next_frame -= 1000;
        mvprintw(0, 0, "Step: %4llu", step);
        printw("  Alive: %7u", prev.alive);
        printw("  Max: %7u", max);
        printw("  Delay: %7ums", delay / 1000);
        refresh();
    }
    freeField(&prev);
    freeField(&next);
    nocbreak();
    endwin();
    echo();
    return 0;
}
