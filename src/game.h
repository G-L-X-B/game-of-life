#pragma once
#ifndef GAME_H
#define GAME_H

#include <inttypes.h>

#include "gameio.h"
#include "life.h"


typedef enum GameMode {
    ST_PLAY,
    ST_EDIT,
} game_mode_t;


typedef struct Game {
    Field_t *cur;
    Field_t *next;
    uint64_t step;
    uint64_t max_alive;
    uint32_t height;
    uint32_t width;
    unsigned iter_delay;    // Delay between iterations in microseconds.
    // Time left before the next iteration.
    unsigned time_to_next;
    game_mode_t state;
} Game_t;


int initGame(Game_t *g, uint32_t h, uint32_t w, uint32_t delay);
/*
 * Free all the data allocated by the initGame.
 * Don't forget to erase all pointers to the dynamic data that
 * you want to save.
 */
void freeGame(Game_t *g);

int game_iterate(Game_t *game);

int editmode(Game_t *game, OutputQueue_t *q, InputEvent_t e);

/*
 * Start simulation - get and process input, lf_iterate the game
 * and print on the screen.
 */
int simulate(Game_t *game, OutputQueue_t *q);

#endif  // GAME_H
