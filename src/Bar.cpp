#include "Bar.h"
#include "main.h"

Bar::Bar(float x, float y, float z, float cx, float cy, float cz, color_t color, float valMax) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->ma = valMax;
    this->scalar = glm::vec3(0,0,1);
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[] = {
        cx-1.0f,cy-1.0f,cz+0, // triangle 1 : begin
        cx-1.0f,cy+1.0f, cz+0,
        cx+1.0f, cy+1.0f, cz+0, // triangle 1 : end
        cx+1.0f, cy+1.0f,cz+0, // triangle 2 : begin
        cx-1.0f,cy-1.0f,cz+0,
        cx+1.0f, cy-1.0f,cz+0, // triangle 2 : end
    };
    
    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, 1, NULL,GL_FILL);
}

void Bar::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 scale     = glm::scale(this->scalar);
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(Matrices.modelMatrixID, 1, GL_FALSE, &Matrices.model[0][0]);
    draw3DObject(this->object);
}

void Bar::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Bar::tick() {
    // this->rotation += speed;
    this->position.x -= speed;
    // this->position.y -= speed;
}

