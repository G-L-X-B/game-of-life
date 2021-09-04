#pragma once
#ifndef LIFE_H
#define LIFE_H

#include <inttypes.h>

typedef enum CellState {
	CS_DEAD,
	CS_ALIVE,
} cellstate_t;


typedef struct Cell {
	cellstate_t state;
} Cell_t;


typedef struct Field {
	Cell_t **field;
	uint32_t height;
	uint32_t width;
} Field_t;


int initField(Field_t *f, uint32_t h, uint32_t w);
void freeField(Field_t *f);

Cell_t *getCell(const Field_t *f, int64_t y, int64_t x);

unsigned neighbours(const Field_t *f, uint32_t y, uint32_t x);

uint32_t iterate(const Field_t *prev, Field_t *next);

#endif