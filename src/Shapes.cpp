#include "main.h"

struct VAO* GenFrustrum( float r1, float r2, color_t color1, color_t color2, color_t color3, float length, int  n){
    GLfloat vertex_buffer_data[100000];
    GLfloat colorBuffer[100000];
    int c = 0;

    glm::vec3 pos(r1,0,length/2);
    glm::vec3 posb(r2,0,-length/2);
    glm::vec3 temp = pos;
    float angle = 360/(float)n;

    for(int i=0;i<n;i++){
        vertex_buffer_data[c++] = pos.x;
        vertex_buffer_data[c++] = pos.y;
        vertex_buffer_data[c++] = pos.z;

        vertex_buffer_data[c++] = posb.x;
        vertex_buffer_data[c++] = posb.y;
        vertex_buffer_data[c++] = posb.z; 

        glm::mat4 rot = glm::rotate(glm::radians(angle), glm::vec3(0,0,1));
        glm::vec4 hpos(pos, 1);
        hpos = rot*hpos;
        pos.x = hpos.x;
        pos.y = hpos.y;
        pos.z = hpos.z;


        vertex_buffer_data[c++] = pos.x;
        vertex_buffer_data[c++] = pos.y;
        vertex_buffer_data[c++] = pos.z; 
        
        vertex_buffer_data[c++] = posb.x;
        vertex_buffer_data[c++] = posb.y;
        vertex_buffer_data[c++] = posb.z; 

        hpos = glm::vec4(posb, 1);
        hpos = rot*hpos;
        posb.x = hpos.x;
        posb.y = hpos.y;
        posb.z = hpos.z;

        vertex_buffer_data[c++] = posb.x;
        vertex_buffer_data[c++] = posb.y;
        vertex_buffer_data[c++] = posb.z;

        vertex_buffer_data[c++] = pos.x;
        vertex_buffer_data[c++] = pos.y;
        vertex_buffer_data[c++] = pos.z;  
    }
    int j = 0;
    for(;j<c;j+=3)
        colorBuffer[j] = (color2.r)/255.0f,
        colorBuffer[j+1] = (color2.g)/255.0f,
        colorBuffer[j+2] = (color2.b)/255.0f;

    pos = glm::vec3(r2, 0, -length/2);
    for(int i=0;i<n;i++){
        vertex_buffer_data[c++] = pos.x;
        vertex_buffer_data[c++] = pos.y;
        vertex_buffer_data[c++] = pos.z;

        vertex_buffer_data[c++] = 0;
        vertex_buffer_data[c++] = 0;
        vertex_buffer_data[c++] = pos.z;

        glm::mat4 rot = glm::rotate(glm::radians(angle), glm::vec3(0,0,1));
        glm::vec4 hpos(pos, 1);
        hpos = rot*hpos;
        pos.x = hpos.x;
        pos.y = hpos.y;
        pos.z = hpos.z;

        vertex_buffer_data[c++] = pos.x;
        vertex_buffer_data[c++] = pos.y;
        vertex_buffer_data[c++] = pos.z;       
    }

    for(;j<c;j+=3)
        colorBuffer[j] = (color1.r)/255.0f,
        colorBuffer[j+1] = (color1.g)/255.0f,
        colorBuffer[j+2] = (color1.b)/255.0f;

    pos = glm::vec3(r1, 0, length/2);
    for(int i=0;i<n;i++){
        vertex_buffer_data[c++] = pos.x;
        vertex_buffer_data[c++] = pos.y;
        vertex_buffer_data[c++] = pos.z;

        vertex_buffer_data[c++] = 0;
        vertex_buffer_data[c++] = 0;
        vertex_buffer_data[c++] = pos.z;

        glm::mat4 rot = glm::rotate(glm::radians(angle), glm::vec3(0,0,1));
        glm::vec4 hpos(pos, 1);
        hpos = rot*hpos;
        pos.x = hpos.x;
        pos.y = hpos.y;
        pos.z = hpos.z;

        vertex_buffer_data[c++] = pos.x;
        vertex_buffer_data[c++] = pos.y;
        vertex_buffer_data[c++] = pos.z;       
    }
    
    for(;j<c;j+=3)
        colorBuffer[j] = (color3.r)/255.0f,
        colorBuffer[j+1] = (color3.g)/255.0f,
        colorBuffer[j+2] = (color3.b)/255.0f;

    return create3DObject(GL_TRIANGLES, c/3, vertex_buffer_data, colorBuffer, GL_FILL);
}