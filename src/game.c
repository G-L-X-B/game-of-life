#include "game.h"

#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "gameio.h"
#include "life.h"


int initSimulation(Simulation_t *s, uint32_t h, uint32_t w, uint32_t delay)
{
    if (!s || h == 0 || w == 0)
        return -1;
    s->cur = malloc(sizeof(*s->cur));
    s->next = malloc(sizeof(*s->next));
    initField(s->cur, h, w);
    initField(s->next, h, w);
    s->step = 0;
    s->max_alive = 0;
    s->iter_delay = delay;
    s->time_to_next = delay;
    s->state = ST_EDIT;
    return 0;
}

void freeSimulation(Simulation_t *s)
{
    if (s->cur) {
        freeField(s->cur);
        free(s->cur);
    }
    if (s->next) {
        freeField(s->next);
        free(s->next);
    }
}


int sim_iterate(Simulation_t *sim)
{
    sim->time_to_next = sim->iter_delay;
    iterate(sim->cur, sim->next);
    Field_t *f = sim->cur;
    sim->cur = sim->next;
    sim->next = f;
    if (sim->cur->alive > sim->max_alive)
        sim->max_alive = sim->cur->alive;
    ++sim->step;
    return 0;
}


// FIXME: Should handle properly max alive overflow while editing the field.
int editmode(Simulation_t *sim, InputEvent_t e)
{
    static uint32_t cury = 10, curx = 10;
    unhighlightCell(sim, cury, curx);
    switch (e.type) {
    case IET_UP:
        cury = cury == 0 ? (uint32_t)LINES - 2 : cury - 1;
        break;
    case IET_DOWN:
        cury = (cury + 1) % (LINES - 1);
        break;
    case IET_LEFT:
        curx = curx == 0 ? (uint32_t)COLS - 1 : curx - 1;
        break;
    case IET_RIGHT:
        curx = (curx + 1) % (COLS - 1);
        break;
    case IET_SWITCH:
        /*
         * Shit's bugged. Got an error about the line after
         * that label cannot be a part of declaration,
         * as it's not a statement.
         */
        curx = curx;
        Cell_t *c = getCell(sim->cur, cury, curx);
        if (c->state == CS_ALIVE) {
            c->state = CS_DEAD;
            --sim->cur->alive;
        } else {
            c->state = CS_ALIVE;
            ++sim->cur->alive;
        }
        break;
    default:
        break;
    }
    highlightCell(sim, cury, curx);
    return 0;
}


int simulate(Simulation_t *sim)
{
    Cell_t **f = sim->cur->field;
    f[0][1].state = CS_ALIVE;
    f[1][1].state = CS_ALIVE;
    f[1][0].state = CS_ALIVE;
    f[2][1].state = CS_ALIVE;
    f[2][2].state = CS_ALIVE;
    sim->cur->alive = 5;
    sim->max_alive = 5;

    InputEvent_t event = {IET_NONE, 0};
    printField(sim->cur);
    while (event.type != IET_STOP) {
        if (sim->state == ST_EDIT) {
            editmode(sim, event);
        } else if (sim->state == ST_PLAY && sim->time_to_next <= 0) {
            sim_iterate(sim);
            printField(sim->cur);
        }
        printStatusBar(sim);
        refresh();

        switch ((event = getinput()).type) {
        case IET_OTHER:
        case IET_PAUSE:
            sim->state = ST_EDIT;
            break;
        case IET_STOP:
            sim->state = ST_EDIT;
            break;
        case IET_CONTINUE:
            sim->state = ST_PLAY;
            break;
        case IET_SPEED_UP:
            sim->iter_delay = sim->iter_delay < 10000 ?
                              0 :
                              sim->iter_delay - 10000;
            sim->time_to_next = sim->time_to_next < 10000 ?
                              0 :
                              sim->time_to_next - 10000;
            break;
        case IET_SLOW_DOWN:
            sim->iter_delay += 10000;
            sim->time_to_next += 10000;
            break;
        case IET_NONE:
        default:
            break;
        }
        usleep(1000);
        if (sim->state == ST_PLAY)
            sim->time_to_next = sim->time_to_next < 1000 ?
                                0 :
                                sim->time_to_next - 1000;
    }
    return 0;
}
