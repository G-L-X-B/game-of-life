#pragma once
#ifndef GAME_H
#define GAME_H

#include <inttypes.h>

#include "life.h"


typedef enum SimulationMode {
    ST_PLAY,
    ST_EDIT,
} sim_mode_t;


typedef struct Simulation {
    Field_t *cur;
    Field_t *next;
    uint64_t step;
    uint64_t max_alive;
    uint32_t height;
    uint32_t width;
    unsigned iter_delay;    // Delay between iterations in microseconds.
    // Time left before the next iteration.
    unsigned time_to_next;
    sim_mode_t state;
} Simulation_t;


int initSimulation(Simulation_t *s, uint32_t h, uint32_t w, uint32_t delay);
/*
 * Free all the data allocated by the initSimulation.
 * Don't forget to erase all pointers to the dynamic data that
 * you want to save.
 */
void freeSimulation(Simulation_t *s);

int sim_iterate(Simulation_t *sim);

typedef struct InputEvent InputEvent_t;
int editmode(Simulation_t *sim, InputEvent_t e);

/*
 * Start simulation - get and process input, iterate the game
 * and print on the screen.
 */
int simulate(Simulation_t *sim);

#endif  // GAME_H