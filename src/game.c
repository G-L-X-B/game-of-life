#include "game.h"

#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "gameio.h"
#include "life.h"
#include "point2d.h"


int initSimulation(Simulation_t *s, uint32_t h, uint32_t w, uint32_t delay)
{
    if (!s || h == 0 || w == 0)
        return -1;
    s->cur = malloc(sizeof(*s->cur));
    s->next = malloc(sizeof(*s->next));
    s->height = h;
    s->width = w;
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


int editmode(Simulation_t *sim, InputEvent_t e)
{
    static point2d_t cur = {0, 0};
    unhighlightCell(sim, cur);
    int infl = sim->max_alive == sim->cur->alive;
    switch (e.type) {
    case IET_UP:
        cur.y = cur.y == 0 ? (uint32_t)sim->height - 1 : cur.y - 1;
        break;
    case IET_DOWN:
        cur.y = (cur.y + 1) % (sim->height);
        break;
    case IET_LEFT:
        cur.x = cur.x == 0 ? (uint32_t)sim->width - 1 : cur.x - 1;
        break;
    case IET_RIGHT:
        cur.x = (cur.x + 1) % (sim->width);
        break;
    case IET_SWITCH:
        /*
         * Shit's bugged. Got an error about the line after
         * that label cannot be a part of declaration,
         * as it's not a statement.
         */
        cur.x = cur.x;
        Cell_t *c = getCell(sim->cur, (point2d_t){cur.y, cur.x});
        if (c->state == CS_ALIVE) {
            c->state = CS_DEAD;
            --sim->cur->alive;
            sim->max_alive -= infl * 1;
        } else {
            c->state = CS_ALIVE;
            ++sim->cur->alive;
            sim->max_alive += infl * 1;
        }
        break;
    default:
        break;
    }
    highlightCell(sim, cur);
    return 0;
}


int simulate(Simulation_t *sim)
{
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
