#include "gamei.h"

#include <ncurses.h>

#include "logger.h"


InputEvent_t io_getinput()
{
    InputEvent_t e;
    switch (e.input = getch()) {
    case ERR:
        e.type = IET_NONE;
        break;
    case 's':
        e.type = IET_PAUSE;
        break;
    case 'q':
        e.type = IET_QUIT;
        break;
    case 'c':
        e.type = IET_CONTINUE;
        break;
    case '+':
        e.type = IET_SPEED_UP;
        break;
    case '-':
        e.type = IET_SLOW_DOWN;
        break;
    case KEY_UP:
        e.type = IET_UP;
        break;
    case KEY_DOWN:
        e.type = IET_DOWN;
        break;
    case KEY_LEFT:
        e.type = IET_LEFT;
        break;
    case KEY_RIGHT:
        e.type = IET_RIGHT;
        break;
    case ' ':
        e.type = IET_SWITCH;
        break;
    default:
        lg_info("Input categorized as \"other\" event.");
        e.type = IET_OTHER;
        break;
    }
    return e;
}
