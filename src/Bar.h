#include "main.h"

#ifndef Bar_H
#define Bar_H


class Bar {
public:
    Bar() {}
    Bar(float x, float y, float z, float cx, float cy, float cz, color_t color, float valMax);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    float ma;
    glm::vec3 scalar;
private:
    VAO *object;
};

#endif // Bar_H
