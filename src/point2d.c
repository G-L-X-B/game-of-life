#include "point2d.h"


point2d_t p2d_add(point2d_t a, point2d_t b)
{
    return (point2d_t){a.y + b.y, a.x + b.x};
}

point2d_t p2d_subract(point2d_t a, point2d_t b)
{
    return (point2d_t){a.y - b.y, a.x - b.x};
}
