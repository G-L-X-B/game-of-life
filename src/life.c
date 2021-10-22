#include "life.h"

#include <stdlib.h>

int initField(Field_t *f, uint32_t h, uint32_t w)
{
    if (!f || h == 0 || w == 0)
        return -1;
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


Cell_t *getCell(const Field_t *f, point2d_t xy)
{
    if (!f)
        return NULL;

    xy.y = (xy.y < 0 ? f->height : 0) + xy.y % f->height;
    xy.x = (xy.x < 0 ? f->width : 0) + xy.x % f->width;
    return &f->field[xy.y][xy.x];
}

unsigned neighbours(const Field_t *f, point2d_t xy)
{
    if (!f || xy.y > f->height || xy.x > f->width)
        return 0;

    const point2d_t shifts[8] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1}, {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };
    unsigned neighbours = 0;
    for (unsigned i = 0; i < 8; ++i)
        if (getCell(
                f,
                (point2d_t){xy.y + shifts[i].y, xy.x + shifts[i].x}
            )->state == CS_ALIVE)
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
            unsigned neighs = neighbours(prev, (point2d_t){y, x});
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
