#include "main.h"
#include "Plane.h"

float heliCamInitDist = -5;
void planeCam(Plane* plane, glm::vec3 cam, glm::vec3 camLook, glm::vec3 camUp){
    glm::vec3 eye = plane->TransformAsPlane(cam);
    glm::vec3 target = plane->TransformAsPlane(camLook);//(plane->position.x, plane->position.y, plane->position.z);
    glm::vec3 up = plane->TransformAsPlane(camUp) - plane->position;
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
}

void topCam(Plane* plane, float height){
    glm::vec3 eye = plane->position;
    eye.z += height;
    glm::vec3 target = plane->position;//(plane->position.x, plane->position.y, plane->position.z);
    glm::vec3 up = glm::vec3(1,0,0);
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
}

void towerCam(Plane* plane){
    glm::vec3 eye(10, 10, 100);
    glm::vec3 target = plane->position;
    glm::vec3 up(0,0,1);

    glm::vec3 tmp = target - eye;
    if(tmp.x == 0 && tmp.y==0) up = glm::vec3(1,0,0);

    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
}

void helicopterCam(GLFWwindow* window, Plane* plane){
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    
    if(yOffset == 1) heliCamInitDist += 0.1;
    else if(yOffset == -1) heliCamInitDist -=0.1;
    heliCamInitDist = std::min(heliCamInitDist, -2.0f);
    heliCamInitDist = std::max(heliCamInitDist, -5.0f);
    yOffset = 0;
    
    std::cout << x << " " << y << std::endl;
    glm::vec4 oeye = glm::vec4(heliCamInitDist,0,0,1);
    glm::vec4 oup = glm::vec4(0,0,-1,1);

    float rotz = x/800 * 360;
    float roty = y/600 * 360;
    glm::mat4 rz = glm::rotate(glm::radians(rotz), glm::vec3(0,0,1));
    glm::mat4 ry = glm::rotate(glm::radians(roty), glm::vec3(0,1,0));
    
    oeye = rz*ry*oeye;
    oup  = rz*ry*oup;
    glm::vec3 eye(oeye.x, oeye.y, oeye.z);
    eye += plane->position;
    glm::vec3 target = plane->position;
    glm::vec3 up(oup.x, oup.y, oup.z);

    Matrices.view = glm::lookAt( eye, target, up ); 
}