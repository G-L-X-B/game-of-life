#pragma once
#ifndef GAMEI_H
#define GAMEI_H


typedef enum InputEventType {
    IET_NONE,
    IET_PAUSE,       // Pause the iteration.
    IET_QUIT,        // Stop the simulation.
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
InputEvent_t io_getinput();

#endif  // GAMEI_H
