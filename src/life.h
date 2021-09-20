#pragma once
#ifndef LIFE_H
#define LIFE_H

#include <inttypes.h>

typedef enum CellState {
    CS_DEAD,
    CS_ALIVE,
} cellstate_t;


typedef enum LifeError {
    LE_OK,
    LE_DIFF_DIM,    // if arguments of iterate have different dimensions
} life_error_t;


typedef struct Cell {
	cellstate_t state;
} Cell_t;


typedef struct Field {
    Cell_t **field;
    uint64_t alive;
    uint32_t height;
    uint32_t width;
} Field_t;


int initField(Field_t *f, uint32_t h, uint32_t w);
void freeField(Field_t *f);

/*
 * Handles out-of bounds indexing by cycling indexes from top to bottom,
 * from left to rigth and vice versa.
 */
Cell_t *getCell(const Field_t *f, int64_t y, int64_t x);

unsigned neighbours(const Field_t *f, uint32_t y, uint32_t x);

life_error_t iterate(const Field_t *prev, Field_t *next);

#endif