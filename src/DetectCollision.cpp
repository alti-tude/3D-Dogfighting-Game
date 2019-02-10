#include "main.h"
#ifndef DETECT_COLLISION_H
#define DETECT_COLLISION_H


bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

#endif