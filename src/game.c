#include "game.h"

#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "gameio.h"
#include "life.h"
#include "point2d.h"


int initGame(Game_t *s, uint32_t h, uint32_t w, uint32_t delay)
{
    if (!s || h == 0 || w == 0)
        return -1;
    s->cur = malloc(sizeof(*s->cur));
    s->next = malloc(sizeof(*s->next));
    s->height = h;
    s->width = w;
    lf_initField(s->cur, h, w);
    lf_initField(s->next, h, w);
    s->step = 0;
    s->max_alive = 0;
    s->iter_delay = delay;
    s->time_to_next = delay;
    s->state = ST_EDIT;
    return 0;
}

void freeGame(Game_t *s)
{
    if (s->cur) {
        lf_freeField(s->cur);
        free(s->cur);
    }
    if (s->next) {
        lf_freeField(s->next);
        free(s->next);
    }
}


int game_iterate(Game_t *game)
{
    game->time_to_next = game->iter_delay;
    lf_iterate(game->cur, game->next);
    Field_t *f = game->cur;
    game->cur = game->next;
    game->next = f;
    if (game->cur->alive > game->max_alive)
        game->max_alive = game->cur->alive;
    ++game->step;
    return 0;
}


int editmode(Game_t *game, OutputQueue_t *q, InputEvent_t e)
{
    if (!game || !q)
        return -1;
    static point2d_t cur = {0, 0};
    /*
     * Defines, whether current change of cell state has influence on
     * max counter or not.
     */
    int infl = game->max_alive == game->cur->alive;
    if (e.type == IET_UP || e.type == IET_DOWN
        || e.type == IET_LEFT || e.type == IET_RIGHT)
        io_qRmCellHighlight(q, cur);
    switch (e.type) {
    case IET_UP:
        cur.y = cur.y == 0 ? (uint32_t)game->height - 1 : cur.y - 1;
        break;
    case IET_DOWN:
        cur.y = (cur.y + 1) % (game->height);
        break;
    case IET_LEFT:
        cur.x = cur.x == 0 ? (uint32_t)game->width - 1 : cur.x - 1;
        break;
    case IET_RIGHT:
        cur.x = (cur.x + 1) % (game->width);
        break;
    case IET_SWITCH:
        /*
         * Shit's bugged. Got an error about the line after
         * next that label cannot be a part of declaration,
         * as it's not a statement.
         */
        infl = infl;
        Cell_t *c = lf_getCell(game->cur, cur);
        lf_invertCellState(c);
        if (c->state == CS_ALIVE) {
            ++game->cur->alive;
            game->max_alive += infl * 1;
        } else {
            --game->cur->alive;
            game->max_alive -= infl * 1;
        }
        io_qPosUpdate(q, game->cur, cur);
        break;
    default:
        break;
    }
    /*
     * Highlight position if there's any change on the screen or the game
     * is in the pre-start state.
     */
    if (e.type == IET_UP || e.type == IET_DOWN || e.type == IET_LEFT
        || e.type == IET_RIGHT || e.type == IET_SWITCH)
        io_qCellHighlight(q, cur);
    return 0;
}


int simulate(Game_t *game, OutputQueue_t *q)
{
    InputEvent_t event = {IET_NONE, 0};
    // As the game starts in editing mode, following print will just put
    // current field state on a screen without doing job twice.
    io_qFieldUpdate(q, game->cur);
    while (event.type != IET_QUIT) {
        if (game->state == ST_EDIT) {
            editmode(game, q, event);
        } else if (game->state == ST_PLAY && game->time_to_next <= 0) {
            game_iterate(game);
            io_qFieldUpdate(q, game->cur);
        }
        io_qStatusBarUpdate(q,
                            game->step,
                            game->cur->alive,
                            game->max_alive,
                            game->iter_delay);
        io_executeQueue(q);

        switch ((event = io_getinput()).type) {
        case IET_OTHER:
        case IET_PAUSE:
            game->state = ST_EDIT;
            break;
        case IET_QUIT:
            game->state = ST_EDIT;
            break;
        case IET_CONTINUE:
            game->state = ST_PLAY;
            break;
        case IET_SPEED_UP:
            game->iter_delay = game->iter_delay < 10000 ?
                               0 :
                               game->iter_delay - 10000;
            game->time_to_next = game->time_to_next < 10000 ?
                                 0 :
                                 game->time_to_next - 10000;
            break;
        case IET_SLOW_DOWN:
            game->iter_delay += 10000;
            game->time_to_next += 10000;
            break;
        case IET_NONE:
        default:
            break;
        }
        usleep(1000);
        if (game->state == ST_PLAY)
            game->time_to_next = game->time_to_next < 1000 ?
                                 0 :
                                 game->time_to_next - 1000;
    }
    return 0;
}
