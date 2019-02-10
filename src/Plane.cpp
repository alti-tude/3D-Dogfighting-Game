#include "Plane.h"
#include "main.h"

Plane::Plane(float x, float y, float z, float r, float length, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotationy = 90;
    this->rotationx = 0;
    this->rotationz = 0;
    this->dir = glm::vec4(0,0,10, 1); 
    this->speed = glm::vec3(0,0,0);
    this->rotMat = glm::rotate((float) (this->rotationy * M_PI / 180.0f), glm::vec3(0, 1, 0));
    PostDraw();
    // this->rotationy = 0;

    this->object = GenFrustrum(r, r, COLOR_RED, COLOR_BLACK, COLOR_RED, length, 4);
    this->object2 = GenFrustrum(0.001, r, COLOR_RED, COLOR_BLACK, COLOR_RED, length/4, 4); 
    this->relPosition2 = glm::vec3(0,0,length/2+length/8);
}

void Plane::draw(glm::mat4 VP) {

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    std::cout << this->rotationy << std::endl;

    glm::mat4 rotatey   = glm::rotate((float) (this->rotationy * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 rotatex   = glm::rotate( (float) (this->rotationx * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 rotatez   = glm::rotate( (float) (this->rotationz * M_PI / 180.0f), glm::vec3(0, 0, 1));
    this->rotMat *=  rotatex * rotatey * rotatez;
    
    Matrices.model *= (translate* this->rotMat);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1,GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);


    glm::vec3 tmp = this->rotMat *glm::vec4(this->relPosition2, 1);
    translate = glm::translate(glm::vec3(tmp.x, tmp.y, tmp.z)+this->position);
    glm::mat4 MVP2 = VP * translate * this->rotMat;
    glUniformMatrix4fv(Matrices.MatrixID, 1,GL_FALSE, &MVP2[0][0]);
    draw3DObject(this->object2);
}
 
void Plane::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

glm::vec3 Plane::TransformAsPlane(glm::vec3 ip){
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
   
    Matrices.model *= (translate * this->rotMat);
    glm::vec4 tmp = Matrices.model*glm::vec4(ip,1);
    return glm::vec3(tmp.x, tmp.y, tmp.z);
}

void Plane::tick() {
    // this->rotation += speed;
    this->position.x += speed.x;
    this->position.y += speed.y;
    this->position.z += speed.z;
    
    // this->position.y -= speed;
}

void Plane::PostDraw(){
    this->rotationx = this->rotationy = this->rotationz = 0;    
}
