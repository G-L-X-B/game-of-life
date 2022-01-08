#include "gameio.h"

#include <stdio.h>  // for `sprintf` in `qStatusBarUpdate`
#include <stdlib.h>

#include <ncurses.h>

#include "logger.h"


char io_cellChar(const CharMap_t *cmap , const Cell_t cell)
{
    int ch;
    switch (cell.state) {
    case CS_ALIVE:
        ch = cmap->alive;
        break;
    case CS_DEAD:
        ch = cmap->dead;
        break;
    default:
        lg_warn("Cell state categorized as \"unknown\".");
        ch = cmap->unknown;
        break;
    }
    return ch;
}


int io_isValidScreenFieldPos(const point2d_t pos)
{
    if (pos.y < 0 || pos.y > LINES - 1
     || pos.x < 0 || pos.x > COLS)
        return 0;
    return 1;
}


OutputAction_t *io_newAction(oat_t type)
{
    OutputAction_t *act = calloc(1, sizeof(*act));
    act->type = type;
    return act;
}

void io_freeAction(OutputAction_t *act)
{
    free(act);
}


io_error_t io_initOutputQueue(OutputQueue_t *q, CharMap_t map, int highlight_filter, point2d_t shift)
{
    if (!q) {
        lg_fatal("Tryied to initialize queue at NULL.");
        return IOE_INV_ARG;
    }
    q->charmap = map;
    q->highlight_filter = highlight_filter;
    q->fshift = shift;
    q->first = q->last = NULL;
    return IOE_OK;
}

/*
 * Push event to the end of the queue
 */
int io_qAction(OutputQueue_t *q, OutputAction_t *act)
{
    if (!q || !act) {
        lg_inv_argument("io_qAction");
        return IOE_INV_ARG;
    }
    if (q->first)
        q->last->next = act;
    else
        q->first = act;
    q->last = act;
    act->next = NULL;
    // lg_debug("Queue first: %p, last: %p.", q->first, q->last);
    return IOE_OK;
}

/*
 * Remove and return the first element from queue.
 */
OutputAction_t *io_popAction(OutputQueue_t *q)
{
    if (!q) {
        lg_inv_argument("io_popAction");
        return NULL;
    }
    if (!q->first)
        return NULL;
    OutputAction_t *ret = q->first;
    q->first = ret->next;
    if (!q->first)
        q->last = NULL;
    ret->next = NULL;
    return ret;
}


io_error_t io_qFieldUpdate(OutputQueue_t *q, const Field_t *f)
{
    if (!q || !f) {
        lg_inv_argument("io_qFieldUpdate");
        return IOE_INV_ARG;
    }
    OutputAction_t *a = io_newAction(IOA_UD_FIELD);
    a->other = f;
    io_qAction(q, a);
    return IOE_OK;
}

io_error_t io_qStatusBarUpdate(OutputQueue_t *q, uint64_t step, uint64_t alive,
                               uint64_t max_alive, unsigned iter_delay)
{
    if (!q) {
        lg_inv_argument("io_qStatusBerUpdate");
        return IOE_INV_ARG;
    }
    // Status bar is 52 characters long
    sprintf(q->statusbar_buf,
            "Step: %4lu  Alive: %5lu  Max: %5lu  Delay: %5ums",
            step,
            alive,
            max_alive,
            iter_delay / 1000);
    io_qAction(q, io_newAction(IOA_UD_STBAR));
    return IOE_OK;
}

io_error_t io_qCellHighlight(OutputQueue_t *q, const point2d_t pos)
{
    if (!q || !io_isValidScreenFieldPos(pos)) {
        lg_inv_argument("io_qCellHighlight");
        return IOE_INV_ARG;
    }
    OutputAction_t *a = io_newAction(IOA_HIGHLIGHT);
    a->pos = pos;
    ++a->pos.y;
    io_qAction(q, a);
    return IOE_OK;
}

io_error_t io_qRmCellHighlight(OutputQueue_t *q, const point2d_t pos)
{
    if (!q || !io_isValidScreenFieldPos(pos)) {
        lg_inv_argument("io_qRmCellHighlight");
        return IOE_INV_ARG;
    }
    OutputAction_t *a = io_newAction(IOA_RM_HIGHLIGHT);
    a->pos = pos;
    ++a->pos.y;
    io_qAction(q, a);
    return IOE_OK;
}

io_error_t io_qPosUpdate(OutputQueue_t *q, const Field_t *f, const point2d_t pos)
{
    if (!q || !f || !io_isValidScreenFieldPos(pos)) {
        lg_inv_argument("io_qPosUpdate");
        return IOE_INV_ARG;
    }
    OutputAction_t *a = io_newAction(IOA_UD_POS);
    a->pos = pos;
    ++a->pos.y;
    a->other = lf_getCellShifted(f, pos, q->fshift);
    io_qAction(q, a);
    return IOE_OK;
}


void io_exFieldUpdate(const OutputQueue_t *q, const Field_t *f)
{
    if (!f) {
        lg_fatal("Field update argument is NULL.");
        return;
    }
    lg_trace("Updating field with source at %p.", f);
    for (unsigned y = 0; y < f->height; ++y) {
        move(y + 1, 0);
        for (unsigned x = 0; x < f->width; ++x)
            addch(
                io_cellChar(
                    &q->charmap,
                    *lf_getCellShifted(f, (point2d_t){y, x}, q->fshift)
                )
            );
    }
}

void io_exStatusBarPrint(const OutputQueue_t *q)
{
    lg_trace("Updating a status bar.");
    mvaddstr(0, 0, q->statusbar_buf);
}

void io_exCellHighlight(const OutputQueue_t *q, const point2d_t pos)
{
    if (!io_isValidScreenFieldPos(pos)) {
        lg_error("Invalid cell highlight position.");
        lg_warn("Ignoring cell highlight due to invalid position argument.");
        return;
    }
    lg_trace("Highlighting cell at (%ld:%ld)", pos.y, pos.x);
    mvchgat(pos.y, pos.x, 1, q->highlight_filter, 0, NULL);
}

void io_exRmCellHighlight(const point2d_t pos)
{
    if (!io_isValidScreenFieldPos(pos)) {
        lg_error("Invalid cell highlight removal position.");
        lg_warn("Ignoring cell highlight removal due to invalid position argument.");
        return;
    }
    lg_trace("Removing cell highlighting at (%ld:%ld)", pos.y, pos.x);
    mvchgat(pos.y, pos.x, 1, A_NORMAL, 0, NULL);
}

void io_exPosUpdate(const OutputQueue_t *q, const Cell_t *cell, const point2d_t pos)
{
    if (!cell) {
        lg_fatal("Cell update argument is NULL.");
        return;
    }
    if (!io_isValidScreenFieldPos(pos)) {
        lg_error("Invalid cell update position.");
        lg_warn("Ignoring cell update due to invalid position argument.");
        return;
    }
    lg_trace("Updating cell at (%ld:%ld)", pos.y, pos.x);
    mvaddch(pos.y, pos.x, io_cellChar(&q->charmap, *cell));
}

io_error_t io_executeQueue(OutputQueue_t *q)
{
    if (!q) {
        lg_inv_argument("io_executeQueue");
        return IOE_INV_ARG;
    }
    OutputAction_t *cur;
    while ((cur = io_popAction(q))) {
        switch (cur->type) {
        case IOA_UD_FIELD:
            io_exFieldUpdate(q, (Field_t*)cur->other);
            break;
        case IOA_UD_STBAR:
            io_exStatusBarPrint(q);
            break;
        case IOA_HIGHLIGHT:
            io_exCellHighlight(q, cur->pos);
            break;
        case IOA_RM_HIGHLIGHT:
            io_exRmCellHighlight(cur->pos);
            break;
        case IOA_UD_POS:
            io_exPosUpdate(q, (Cell_t*)cur->other, cur->pos);
            break;
        default:
            break;
        }
        io_freeAction(cur);
    }
    refresh();
    return IOE_OK;
}
