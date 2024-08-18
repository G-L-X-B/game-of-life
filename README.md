# game-of-life
Conway's Game of Life in the console using `ncurses`.

## Why?

Game of Life simulation is an interesting program to write for yourself yet simple enough for new features features to be added. A perfect educational project you can always find something to improve in or to add to.

## Building

This will download repository with it's dependencies and build a `game-of-life` executable.

```
$ git clone ...
$ cd game-of-life/src
$ git submodule init
$ git submodule update
$ make
```

## Dependencies

You must have `ncurses` library installed in your system.

Also, [log.c](https://github.com/rxi/log.c) will be downloaded as a submodule.

## Bugs

Note that versions built under 32-bit platform will have incorrect values in status bar. To fix that, change the format string tokens in the `io_qStatusBarUpdate` from `%4lu` to `%4llu` or whatever your compiler advices you.
