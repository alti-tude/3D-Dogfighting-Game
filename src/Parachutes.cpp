#include "Parachutes.h"
#include "main.h"

Parachute::Parachute(float x, float y, float z, color_t color, float side) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;//rand()%90;
    speed = glm::vec3(0,0,0);

    GLfloat vba[]={
        2*side*sin(glm::radians(54.0f)),-side,2*side*cos(glm::radians(54.0f)),
        0,0,0,
        0.1,0,0,
        2*side*sin(glm::radians(54.0f)),-side,2*side*cos(glm::radians(54.0f)),
        0.1,0,0,
        0.1,0,0.1,
        2*side*sin(glm::radians(54.0f)),-side,2*side*cos(glm::radians(54.0f)),
        0.1,0,0.1,
        0,0,0.1,
        2*side*sin(glm::radians(54.0f)),-side,2*side*cos(glm::radians(54.0f)),
        0,0,0,
        0,0,0.1,
        
        2*side*sin(glm::radians(54.0f)),side,2*side*cos(glm::radians(54.0f)),
        0,0,0,
        0.1,0,0,
        2*side*sin(glm::radians(54.0f)),side,2*side*cos(glm::radians(54.0f)),
        0.1,0,0,
        0.1,0,0.1,
        2*side*sin(glm::radians(54.0f)),side,2*side*cos(glm::radians(54.0f)),
        0.1,0,0.1,
        0,0,0.1,
        2*side*sin(glm::radians(54.0f)),side,2*side*cos(glm::radians(54.0f)),
        0,0,0,
        0,0,0.1,
        
        -2*side*sin(glm::radians(54.0f)),side,2*side*cos(glm::radians(54.0f)),
        0,0,0,
        0.1,0,0,
        -2*side*sin(glm::radians(54.0f)),side,2*side*cos(glm::radians(54.0f)),
        0.1,0,0,
        0.1,0,0.1,
        -2*side*sin(glm::radians(54.0f)),side,2*side*cos(glm::radians(54.0f)),
        0.1,0,0.1,
        0,0,0.1,
        -2*side*sin(glm::radians(54.0f)),side,2*side*cos(glm::radians(54.0f)),
        0,0,0,
        0,0,0.1,

        -2*side*sin(glm::radians(54.0f)),-side,2*side*cos(glm::radians(54.0f)),
        0,0,0,
        0.1,0,0,
        -2*side*sin(glm::radians(54.0f)),-side,2*side*cos(glm::radians(54.0f)),
        0.1,0,0,
        0.1,0,0.1,
        -2*side*sin(glm::radians(54.0f)),-side,2*side*cos(glm::radians(54.0f)),
        0.1,0,0.1,
        0,0,0.1,
        -2*side*sin(glm::radians(54.0f)),-side,2*side*cos(glm::radians(54.0f)),
        0,0,0,
        0,0,0.1,
    };
    this->object = GenFrustrum(sqrt(side), sqrt(side), color, color, color, 1, side, 4, 0, 0, 45);
    this->object2 = GenFrustrumHollow(2*side, 2*side, 0.2*side, COLOR_BLACK, 0.8, 2*side, 360, 0.3, 90, 0, 36);
    this->object3 = create3DObject(GL_TRIANGLES, 48, vba, COLOR_BLACK,1, NULL, GL_FILL);
    this->relPos2 = glm::vec3(0,0,0);
}

void Parachute::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(Matrices.modelMatrixID, 1, GL_FALSE, &Matrices.model[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object3);
    DrawElem(this->object2, relPos2, VP);
}

void Parachute::DrawElem(VAO* obj, glm::vec3 relPos, glm::mat4 VP){
    glm::vec3 tmp = glm::vec4(relPos, 1);
    glm::mat4 translate = glm::translate(glm::vec3(tmp.x, tmp.y, tmp.z)+this->position);
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 MVP2 = VP * translate * rotate;
    glUniformMatrix4fv(Matrices.MatrixID, 1,GL_FALSE, &MVP2[0][0]);
    glUniformMatrix4fv(Matrices.modelMatrixID, 1, GL_FALSE, &Matrices.model[0][0]);
    draw3DObject(obj);
}
void Parachute::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Parachute::tick() {
    this->position += this->speed;
}

