#include "main.h"

#ifndef PLANE_H
#define PLANE_H


class Plane {
public:
    Plane() {}
    Plane(float x, float y, float z, float r, float length, color_t color);
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    glm::vec3 TransformAsPlane(glm::vec3 ip);
    void tick();
    void PostDraw();

public:
    glm::vec3 position;
    glm::vec3 relPosition2;
    glm::vec3 speed;
    glm::vec4 dir;
    glm::mat4 rotMat;
    float rotationx;
    float rotationy;
    float rotationz;
private:
    VAO *object;
    VAO *object2;
};

#endif // Plane_H
