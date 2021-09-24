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
    unsigned iter_delay;    // Delay between iterations in microseconds.
    // Time left before the next iteration.
    unsigned time_to_next;
    sim_mode_t state;
} Simulation_t;


int initSimulation(Simulation_t *s, uint32_t h, uint32_t w, uint32_t delay);
/*
 * Free all the data allocated by the initSimulation.
 * Don't forget to clean up all the pointers to the dynamic data that
 * you want to save.
 */
void freeSimulation(Simulation_t *s);

int sim_iterate(Simulation_t *sim);

#endif  // GAME_H