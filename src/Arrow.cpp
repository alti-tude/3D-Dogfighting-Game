#include "Arrow.h"
#include "main.h"

Arrow::Arrow(float x, float y, float z, float radius, float length, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    
    this->object = GenFrustrum(radius, radius, {255,0,0}, {255,0,0}, {255,0,0}, 0.6, length, 360, 0,0,0);
    this->object2 = GenFrustrum(3*radius/2, 0, {255,0,0}, {255,0,0}, {255,0,0}, 0.6, length/6, 360, 0,0,0);

    this->relPos2 = glm::vec3(0,0,-length/2 - length/12);
}

void Arrow::draw(glm::mat4 VP) {
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
    DrawElem(this->object2, relPos2, VP);
}

void Arrow::DrawElem(VAO* obj, glm::vec3 relPos, glm::mat4 VP){
    glm::vec3 tmp = glm::vec4(relPos, 1);
    glm::mat4 translate = glm::translate(glm::vec3(tmp.x, tmp.y, tmp.z)+this->position);
    glm::mat4 MVP2 = VP * translate;
    glUniformMatrix4fv(Matrices.MatrixID, 1,GL_FALSE, &MVP2[0][0]);
    glUniformMatrix4fv(Matrices.modelMatrixID, 1, GL_FALSE, &Matrices.model[0][0]);
    draw3DObject(obj);
}

void Arrow::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Arrow::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

