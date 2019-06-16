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

    glm::vec3 up = glm::vec3(0,0,0);
    this->object = GenFrustrum(r, r, COLOR_RED, {120, 140, 77}, COLOR_RED, 1, length, 360,0, 0, 0);
    this->object2 = GenFrustrum(0.001, r, COLOR_RED, {140, 128, 77}, COLOR_RED, 1, length/4, 360, 0, 0, 0); 
    this->object3 = GenFrustrum(r/2, r/2, COLOR_RED, {140, 128, 77}, COLOR_RED, 1, length, 4, 90, 0, 45); 

    this->relPosition2 = glm::vec3(0,0,length/2+length/8);
    this->relPosition3 = glm::vec3(0,0,0);

    colPoints.push_back(glm::vec3(0,0,length/2+length/4));
    colPoints.push_back(glm::vec3(0,0,-length/2));
    colPoints.push_back(glm::vec3(0,length/2,0));
    colPoints.push_back(glm::vec3(0,-length/2,0));
    colPoints.push_back(glm::vec3(0,0,0));
    this->adSpeed = this->speed;
    this->storSpeed = TransformAsPlane(this->speed);
}

void Plane::draw(glm::mat4 VP, bool flag) {

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef

    glm::mat4 rotatey   = glm::rotate((float) (this->rotationy * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 rotatex   = glm::rotate((float) (this->rotationx * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 rotatez   = glm::rotate((float) (this->rotationz * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 scaled    = glm::scale(glm::vec3(10,10,10));
    this->rotMat *=  rotatex * rotatey * rotatez;
    
    if(flag) Matrices.model *= (translate* this->rotMat * scaled);
    else Matrices.model *= (translate* this->rotMat);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1,GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(Matrices.modelMatrixID, 1, GL_FALSE, &Matrices.model[0][0]);
    draw3DObject(this->object);

    DrawElem(this->object2, this->relPosition2, VP, flag);
    DrawElem(this->object3, this->relPosition3, VP, flag);
   
}

void Plane::DrawElem(VAO* obj, glm::vec3 relPos, glm::mat4 VP, bool flag){
    glm::vec3 tmp = this->rotMat *glm::vec4(relPos, 1);

    glm::mat4 translate = glm::translate(glm::vec3(tmp.x, tmp.y, tmp.z)+this->position);
    glm::mat4 scaled    = glm::scale(glm::vec3(10,10,10));
    
    if(flag) Matrices.model = (translate* this->rotMat * scaled);
    else Matrices.model = (translate* this->rotMat);

    glm::mat4 MVP2 = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1,GL_FALSE, &MVP2[0][0]);
    glUniformMatrix4fv(Matrices.modelMatrixID, 1, GL_FALSE, &Matrices.model[0][0]);
    draw3DObject(obj);
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

std::vector<glm::vec3 > Plane::GetColPoints(){
    std::vector<glm::vec3 > v;
    for(auto it:this->colPoints){
        v.push_back(TransformAsPlane(it));
    }
    return v;
}
void Plane::tick() {
    // this->rotation += speed;
    this->position = TransformAsPlane(this->speed);     
    // this->storSpeed = adSpeed + TransformAsPlane(this->speed)-this->position;
    // adSpeed.z = storSpeed.z;
    // this->position.y -= speed;
}

void Plane::PostDraw(){
    this->rotationx = this->rotationy = this->rotationz = 0;    
}
