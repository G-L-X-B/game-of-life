#include "life.h"

#include <stdlib.h>
#include <stdio.h>

int initField(Field_t *f, uint32_t h, uint32_t w)
{
    f->alive = 0;
	f->height = h;
	f->width = w;
	f->field = malloc(h * sizeof(*f->field));
	for (uint32_t i = 0; i < h; ++i) {
		f->field[i] = malloc(w * sizeof(**f->field));
		for (uint32_t j = 0; j < w; ++j)
			f->field[i][j].state = CS_DEAD;
	}
	return 0;
}

void freeField(Field_t *f)
{
	for (uint32_t i = 0; i < f->height; ++i)
		free(f->field[i]);
	free(f->field);
}

Cell_t *getCell(const Field_t *f, int64_t y, int64_t x)
{
    if (!f)
        return NULL;

    y = (y < 0 ? f->height : 0) + y % f->height;
    x = (x < 0 ? f->width : 0) + x % f->width;
    return &f->field[y][x];
}

unsigned neighbours(const Field_t *f, uint32_t y, uint32_t x)
{
    if (!f || y > f->height || x > f->width)
        return 0;

    const int shifts[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1}, {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };
    unsigned neighbours = 0;
    for (unsigned i = 0; i < 8; ++i)
        if (getCell(f, (int64_t)y + shifts[i][0], (int64_t)x + shifts[i][1])->state == CS_ALIVE)
            ++neighbours;
    return neighbours;
}

life_error_t iterate(const Field_t *prev, Field_t *next)
{
    if (!prev || !next
        || prev->height != next->height
        || prev->width != next->width)
        return LE_DIFF_DIM;

    next->alive = 0;
    for (uint32_t y = 0; y < prev->height; ++y) {
        for (uint32_t x = 0; x < prev->width; ++x) {
            unsigned neighs = neighbours(prev, y, x);
            if (neighs == 2 && prev->field[y][x].state == CS_ALIVE
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