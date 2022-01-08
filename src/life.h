#pragma once
#ifndef LIFE_H
#define LIFE_H

#include <inttypes.h>

#include "point2d.h"


typedef enum LifeError {
    LE_OK = 0,
    LE_INV_ARG,
    LE_DIFF_DIM,    // if arguments of lf_iterate have different dimensions
} life_error_t;


typedef enum CellState {
    CS_DEAD,
    CS_ALIVE,
} cellstate_t;

typedef struct Cell {
	cellstate_t state;
} Cell_t;


typedef struct Field {
    Cell_t **field;
    uint64_t alive;
    uint32_t height;
    uint32_t width;
} Field_t;


life_error_t lf_initField(Field_t *f, uint32_t h, uint32_t w);
void lf_freeField(Field_t *f);

/*
 * Handles out-of bounds indexing by cycling indexes from top to bottom,
 * from left to rigth and vice versa.
 */
Cell_t *lf_getCell(const Field_t *f, point2d_t pos);
/*
 * Just like lf_getCell, but takes additional argument - a shift.
 */
Cell_t *lf_getCellShifted(const Field_t *f, point2d_t pos, point2d_t shift);
unsigned lf_countNeighbours(const Field_t *f, point2d_t pos);

life_error_t lf_invertCellState(Cell_t *cell);

life_error_t lf_iterate(const Field_t *prev, Field_t *next);

#endif  // LIFE_H
