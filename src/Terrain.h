#ifndef TERRAIN_H
#define TERRAIN_H
#include "main.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct VAO* GenTerrain(int x, int y, float length);

void draw(struct VAO* obj, glm::mat4 VP);
bool CheckCollision(float x, float y, float z, float buffer);
#endif