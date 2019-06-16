#include "Water.h"

Water::Water(float width, float length, float sqSize){
    float vertexBuffer[18] = {
        -width/2*sqSize, -length/2*sqSize, 3,   
        width/2*sqSize, -length/2*sqSize, 3,   
        width/2*sqSize, length/2*sqSize, 3,   
        -width/2*sqSize, -length/2*sqSize, 3,   
        -width/2*sqSize, length/2*sqSize, 3,   
        width/2*sqSize, length/2*sqSize, 3
    };
    float uvBuffer[12] = {
        0,1,
        1,1,
        1,0,
        0,1,
        0,0,
        1,0,
    };
    this->width = width;
    this->length = length;
    this->sqSize = sqSize;
    this->obj = create3DObject(GL_TRIANGLES, 6, vertexBuffer, {0,0,255}, 1, NULL, GL_FILL);
    this->fbo = new FrameBuffer(width, length);
    
    this->shaderID = LoadShaders("textured.vert", "textured.frag");
}

void Water::Draw(glm::mat4 VP){
    glUseProgram(shaderID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo->GetTexture());
    glUniform1i(glGetUniformLocation(shaderID, "textureSampler"), 0);

    glUniform1i(glGetUniformLocation(shaderID, "textureActivated"), 0);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &VP[0][0]);
    draw3DObject(this->obj);

    // glActiveTexture(0);
    glUseProgram(0);
}