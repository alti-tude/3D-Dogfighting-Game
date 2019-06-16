#include "Plane.h"
#include "main.h"

void planeCam(Plane* plane, glm::vec3 cam, glm::vec3 camLook, glm::vec3 camUp); 
void topCam(Plane* plane, float height);
void towerCam(Plane* plane);
void helicopterCam(GLFWwindow* window, Plane* plane);