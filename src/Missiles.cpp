#include "Missiles.h"
#include "main.h"


Missile::Missile(float x, float y, float z, float r, float length, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotationy = 0;
    this->rotationx = 0;
    this->rotationz = 0;
    this->windUp = 10;

    this->speed = glm::vec3(0,0,0.05);
    this->dir = glm::normalize(this->speed);

    this->rotMat = glm::mat4(1.0f);
    // this->rotMat = glm::rotate((float) (this->rotationy * M_PI / 180.0f), glm::vec3(0, 1, 0));
    PostDraw();

    this->object[0] = GenFrustrum(r, r, COLOR_RED, color, COLOR_RED,1, length, 360,0, 0, 0);
    this->object[1] = GenFrustrum(0.001, r, COLOR_RED, COLOR_RED, COLOR_RED,1, length/4, 360, 0, 0, 0); 
    this->object[2] = GenFrustrum(r/2, r/2, COLOR_RED, COLOR_BLACK, COLOR_RED,1, 4*r, 4, 0, 90, 0);
    this->object[3] = GenFrustrum(r/2, r/2, COLOR_RED, COLOR_BLACK, COLOR_RED,1, 4*r, 4, 90, 0, 0); 

    this->relPosition[1] = glm::vec3(0,0,length/2+length/8);
    this->relPosition[2] = glm::vec3(0,0,-length/2);
    this->relPosition[3] = glm::vec3(0,0,-length/2);
    numObj = 4;
}

void Missile::draw(glm::mat4 VP) {
    this->dir = glm::normalize(this->speed);
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef

    glm::vec3 r(0,0,1);
    glm::vec3 r1;
    glm::vec3 r2;
    if(this->dir.x == 0 && this->dir.y == 0) 
        r = glm::vec3(0,1,0);

    r1 = glm::cross(this->dir, r);
    r2 = glm::cross(this->dir, r1);
    // glm::mat4 rotatey   = glm::rotate((float) (this->rotationy * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // glm::mat4 rotatex   = glm::rotate( (float) (this->rotationx * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // glm::mat4 rotatez   = glm::rotate( (float) (this->rotationz * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // this->rotMat *=  rotatex * rotatey * rotatez;
    r1 = glm::normalize(r1);
    r2 = glm::normalize(r2);

    if(dir!= glm::vec3(0,0,0)){
        this->rotMat[0][0] = r2.x, this->rotMat[1][0] = r2.y, this->rotMat[2][0] = r2.z;
        this->rotMat[0][1] = r1.x, this->rotMat[1][1] = r1.y, this->rotMat[2][1] = r1.z;
        this->rotMat[0][2] = this->dir.x, this->rotMat[1][2] = this->dir.y, this->rotMat[2][2] = this->dir.z;
    }

    Matrices.model *= (translate*glm::transpose(this->rotMat));
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1,GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(Matrices.modelMatrixID, 1, GL_FALSE, &Matrices.model[0][0]);
    draw3DObject(this->object[0]);

    for(int i=1;i<4;i++){
        DrawElem(this->object[i], this->relPosition[i], VP);
    }
}

void Missile::DrawElem(VAO* obj, glm::vec3 relPos, glm::mat4 VP){
    glm::vec3 tmp = glm::transpose(this->rotMat) *glm::vec4(relPos, 1);
    glm::mat4 translate = glm::translate(glm::vec3(tmp.x, tmp.y, tmp.z)+this->position);
    glm::mat4 MVP2 = VP * translate*glm::transpose(this->rotMat);
    glUniformMatrix4fv(Matrices.MatrixID, 1,GL_FALSE, &MVP2[0][0]);
    glUniformMatrix4fv(Matrices.modelMatrixID, 1, GL_FALSE, &Matrices.model[0][0]);
    draw3DObject(obj);
}

void Missile::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Missile::tick() {
    windUp--;
    this->dir = glm::normalize(this->speed);
    if(windUp>0) return;
    // this->speed.z -= 0.002;
    this->position += this->speed;
}

void Missile::PostDraw(){
    this->rotationx = this->rotationy = this->rotationz = 0;
}
