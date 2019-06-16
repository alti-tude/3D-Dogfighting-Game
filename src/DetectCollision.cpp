#include "main.h"

bool detectCollision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.xl + b.xl)) &&
           (abs(a.y - b.y) * 2 < (a.yl + b.yl)) &&
           (abs(a.z - b.z) * 2 < (a.zl + b.zl));
}
