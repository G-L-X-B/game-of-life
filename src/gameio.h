#pragma once
#ifndef GAMEIO_H
#define GAMEIO_H

#include <inttypes.h>

#include "gamei.h"
#include "life.h"
#include "point2d.h"


typedef enum IO_Error {
    IOE_INV_ARG = -1,
    IOE_OK = 0,
} io_error_t;


typedef struct CharMap {
    int alive;
    int dead;
    int unknown;
} CharMap_t;

char io_cellChar(const CharMap_t *cmap, const Cell_t cell);


/*
 * True if pos is inside of the rectangle with sizes LINE - 1, COLS.
 */
int io_isValidScreenFieldPos(const point2d_t pos);


typedef enum OutputActionType {
    IOA_UD_FIELD,       // Write field
    IOA_UD_STBAR,       // Write status bar
    IOA_UD_POS,         // Update position of field
    IOA_HIGHLIGHT,
    IOA_RM_HIGHLIGHT,
} oat_t;

/*
 * Holds data about an output action.
 * type - type of action
 * pos - used for highlights and position updates
 * next - pointer to the next action in the queue
 * other - special data for some special cases that is managed by outer scope
 */
typedef struct OutputAction {
    point2d_t pos;
    const void *other;
    struct OutputAction *next;
    oat_t type;
} OutputAction_t;

OutputAction_t *io_newAction(oat_t type);
void io_freeAction(OutputAction_t *act);


typedef struct OutputQueue {
    char statusbar_buf[53];
    CharMap_t charmap;
    int highlight_filter;
    // Reserved for future, rn always {0, 0}
    point2d_t fshift;   // Screen shift of a visible part of the field.

    OutputAction_t *first;
    OutputAction_t *last;
} OutputQueue_t;


io_error_t io_initOutputQueue(OutputQueue_t *q, CharMap_t map, int highlight_filter, point2d_t shift);

/*
 * Following functions add actions into the queue.
 * As an API to the output queue they perform automatic conversion
 * of arguements so ex~ functions just do their job and don't care about
 * anything other than that.
 */

/*
 * Sets the `other` member to be a pointer to the field to print.
 */
io_error_t io_qFieldUpdate(OutputQueue_t *q, const Field_t *f);
io_error_t io_qStatusBarUpdate(OutputQueue_t *q, uint64_t step, uint64_t alive,
                               uint64_t max_alive, unsigned iter_delay);

/*
 * Sets the `pos` member to be a highlight position.
 */
io_error_t io_qCellHighlight(OutputQueue_t *q, const point2d_t pos);
/*
 * Sets the `pos` member to be a position to remove highlight.
 */
io_error_t io_qRmCellHighlight(OutputQueue_t *q, const point2d_t pos);

/*
 * Sets the `pos` to be a position to print updated state on
 * and the `other` member to be a cell to update.
 */
io_error_t io_qPosUpdate(OutputQueue_t *q, const Field_t *f, const point2d_t pos);


io_error_t io_executeQueue(OutputQueue_t *q);


#endif  // GAMEIO_H
