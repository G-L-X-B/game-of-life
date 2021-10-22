#pragma once
#ifndef GAMEIO_H
#define GAMEIO_H

#include "game.h"
#include "life.h"
#include "point2d.h"


typedef struct CharMap {
    int alive;
    int dead;
    int unknown;
} CharMap_t;


char cellChar(const Cell_t cell);

/*
 * Print a status bar. Doen't call refresh.
 */
int printStatusBar(const Simulation_t *s);
int printField(const Field_t *f);
/*
 * Update corresponding character on a screen and highlight it.
 * Be careful: moves cursor to the y + 1, x + 1 coordinates!
 */
int highlightCell(const Simulation_t *sim, const point2d_t pos);
int unhighlightCell(const Simulation_t *sim, const point2d_t pos);


typedef enum InputEventType {
    IET_NONE,
    IET_PAUSE,       // Pause the iteration.
    IET_STOP,        // Stop the simulation.
    IET_CONTINUE,
    IET_SPEED_UP,
    IET_SLOW_DOWN,
    IET_UP,
    IET_DOWN,
    IET_LEFT,
    IET_RIGHT,
    IET_SWITCH,
    IET_OTHER,
} iet_t;


typedef struct InputEvent {
    iet_t type;
    int input;
} InputEvent_t;

/*
 * Validate input from getch and interpret it.
 */
InputEvent_t getinput();

#endif  // GAMEIO_H
