#include "life.h"

#include <stdlib.h>

#include "logger.h"


/*
 * Almoust every logging activity here is commented out just because
 * they produce too many log entries for so little time.
 * (Around 1MB of logs for one iteration over the field with lf_getCellShifted)
 */


life_error_t lf_initField(Field_t *f, uint32_t h, uint32_t w)
{
    if (!f || h == 0 || w == 0) {
        lg_inv_argument("lf_initField");
        return LE_INV_ARG;
    }
    f->alive = 0;
	f->height = h;
	f->width = w;
	f->field = malloc(h * sizeof(*f->field));
	for (uint32_t i = 0; i < h; ++i) {
		f->field[i] = malloc(w * sizeof(**f->field));
		for (uint32_t j = 0; j < w; ++j)
			f->field[i][j].state = CS_DEAD;
	}
    lg_debug("Initialized field at %p with height: %u and width: %u.", f, h, w);
	return LE_OK;
}

void lf_freeField(Field_t *f)
{
    lg_debug("Field to free is at %p.", f);
	for (uint32_t i = 0; i < f->height; ++i)
		free(f->field[i]);
	free(f->field);
}


Cell_t *lf_getCell(const Field_t *f, point2d_t pos)
{
    if (!f) {
        lg_inv_argument("lf_getCell");
        return NULL;
    }
    pos.y = (pos.y < 0 ? f->height : 0) + pos.y % f->height;
    pos.x = (pos.x < 0 ? f->width : 0) + pos.x % f->width;
    //lg_debug("Counted pos as {%ld, %ld}.", pos.y, pos.x);
    return &f->field[pos.y][pos.x];
}

Cell_t *lf_getCellShifted(const Field_t *f, point2d_t pos, point2d_t shift)
{
    return lf_getCell(f, p2d_add(pos, shift));
}

unsigned lf_countNeighbours(const Field_t *f, point2d_t pos)
{
    if (!f || pos.y > f->height || pos.x > f->width) {
        lg_inv_argument("lf_countNeighbours");
        return 0;
    }

    const point2d_t shifts[8] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1}, {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };
    unsigned neighbours = 0;
    for (unsigned i = 0; i < 8; ++i)
        if (lf_getCell(
                f,
                p2d_add(pos, shifts[i])
            )->state == CS_ALIVE)
            ++neighbours;
    //lg_debug("Counted %u neighnours at (%ld, %ld).", neighbours, pos.y, pos.x);
    return neighbours;
}


life_error_t lf_invertCellState(Cell_t *cell)
{
    if (!cell) {
        lg_inv_argument("lf_invertCellState");
        return LE_INV_ARG;
    }
    if (cell->state == CS_ALIVE)
        cell->state = CS_DEAD;
    else
        cell->state = CS_ALIVE;
    return LE_OK;
}


life_error_t lf_iterate(const Field_t *prev, Field_t *next)
{
    if (!prev || !next) {
        lg_inv_argument("lf_iterate");
        return LE_INV_ARG;
    }
    if (prev->height != next->height || prev->width != next->width) {
        lg_error("Given field have non-matching dimensions' sizes.");
        return LE_DIFF_DIM;
    }
    next->alive = 0;
    for (uint32_t y = 0; y < prev->height; ++y) {
        for (uint32_t x = 0; x < prev->width; ++x) {
            unsigned neighs = lf_countNeighbours(prev, (point2d_t){y, x});
            if ((neighs == 2 && prev->field[y][x].state == CS_ALIVE)
                || neighs == 3) {
                next->field[y][x].state = CS_ALIVE;
                ++next->alive;
            } else {
                next->field[y][x].state = CS_DEAD;
            }
        }
    }
    return LE_OK;
}
