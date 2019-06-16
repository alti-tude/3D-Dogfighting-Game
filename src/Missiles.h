#include "main.h"

#ifndef MISSILES_H
#define MISSILES_H


class Missile {
public:
    Missile() {}
    Missile(float x, float y, float z, float r, float length, color_t color);
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    void PostDraw();
    void DrawElem(VAO* obj, glm::vec3 relPos, glm::mat4 VP);

public:
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 dir;
    glm::mat4 rotMat;
    int windUp;
    float rotationx;
    float rotationy;
    float rotationz;

private:
    VAO *object[1000];
    glm::vec3 relPosition[1000];
    int numObj;
};

#endif // MISSILES_H
