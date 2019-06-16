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
using namespace std;

struct VAO* GenTerrain(int x, int y, float length, vector<pair<int,int> >& v, int a);

void Draw(struct VAO* obj, glm::mat4 VP);
bool CheckCollisionGround(float x, float y, float z, float buffer);
bool checkVissible(glm::vec3 from, glm::vec3 to);

extern float stor[900+1][900+1];
#endif