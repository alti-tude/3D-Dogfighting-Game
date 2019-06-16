#include "main.h"

#ifndef PARACHUTE_H
#define PARACHUTE_H


class Parachute {
public:
    Parachute() {}
    Parachute(float x, float y, float z, color_t color, float side);
    void DrawElem(VAO* obj, glm::vec3 relPos, glm::mat4 VP);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    glm::vec3 speed;
private:
    VAO *object;
    VAO *object2;
    VAO *object3;
    glm::vec3 relPos2;
};

#endif // PARACHUTE_H
