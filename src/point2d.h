#pragma once
#ifndef POINT2D_H
#define POINT2D_H

#include <inttypes.h>


typedef struct Point2d {
    int64_t y;
    int64_t x;
} point2d_t;

point2d_t p2d_add(point2d_t a, point2d_t b);
point2d_t p2d_subract(point2d_t a, point2d_t b);

#endif  // POINT2D_H
