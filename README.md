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

## TODO

In case I forget something or it was left for significant time, there's a list of what I want to see done in this project. They are not necessarily ordered, but you can see a progression in there.

- [ ] Separate iteration mode, edit mode and input handling logic one from another.
- [ ] Do something about printing status bar on **every single input check**.
- [ ] There must be something done with error handling. xmalloc, create abort hoocks and other.
- [ ] An understandable logging policy ([e.g. rpcs3 logging](https://github.com/RPCS3/rpcs3/wiki/Coding-Style)).
- [ ] It would be nice if status bar will show context info in different modes. Still, after switching to edit mode status bar should remain untouched until something is changed. Yeah, it must be just like it's a *pause*.
- [ ] Command line arguments! Want to switch logging, setting log level, file, and so on.
- [ ] Field size must not depend on terminal size. And remember that *something*-shift-*something* member in the output queue manager? It determines what part of a larger field should be printed. Also, it must be scrollable.
- [ ] Configuration files. Default settings, input and output characters mapping and so on. There must be a default file present and an opportunity to override it by the cli.
- [ ] Packaging and installation.
- [ ] Tests.
- [ ] Saving state of the field into a file and loading it.
- [ ] A library of patterns.
- [ ] A library of *custom* patterns. Draw, select, save, rotate, place and delete.
