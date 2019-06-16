#include "main.h"

#ifndef PLANE_H
#define PLANE_H


class Plane {
public:
    Plane() {}
    Plane(float x, float y, float z, float r, float length, color_t color);
    void draw(glm::mat4 VP, bool flag);
    void set_position(float x, float y);
    glm::vec3 TransformAsPlane(glm::vec3 ip);
    void tick();
    void PostDraw();
    void DrawElem(VAO* obj, glm::vec3 relPos, glm::mat4 VP, bool flag);
    std::vector<glm::vec3 > GetColPoints();

public:
    glm::vec3 position;
    glm::vec3 relPosition2;
    glm::vec3 relPosition3;
    glm::vec3 speed;
    glm::vec3 storSpeed;
    glm::vec3 adSpeed;
    glm::vec4 dir;
    glm::mat4 rotMat;
    std::vector<glm::vec3 > colPoints;
    float rotationx;
    float rotationy;
    float rotationz;

private:
    VAO *object;
    VAO *object2;
    VAO* object3;
};

#endif // Plane_H
