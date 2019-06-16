#include "main.h"

#ifndef TURRET_H
#define TURRET_H


class Turret {
public:
    Turret() {}
    Turret(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // TURRET_H
