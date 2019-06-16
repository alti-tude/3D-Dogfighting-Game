#include "main.h"

#ifndef ARROW_H
#define ARROW_H


class Arrow {
public:
    Arrow() {}
    Arrow(float x, float y, float z, float radius, float length, color_t color);
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void DrawElem(VAO* obj, glm::vec3 relPos, glm::mat4 VP);
    void tick();

public:
    glm::vec3 position;
    glm::vec3 relPos2;
    double speed;
    float rotation;
private:
    VAO *object;
    VAO *object2;
};

#endif // ARROW_H
