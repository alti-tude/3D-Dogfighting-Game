#include "main.h"

glm::mat4 rotation(float rotx, float roty, float rotz){
    glm::mat4 rotatey   = glm::rotate((float) (roty * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 rotatex   = glm::rotate( (float) (rotx * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 rotatez   = glm::rotate( (float) (rotz * M_PI / 180.0f), glm::vec3(0, 0, 1));
    return rotatex*rotatey*rotatez;
}

struct VAO* GenFrustrum( float r1, float r2, color_t color1, color_t color2, color_t color3, float alpha, float length, int  n, float rotx, float roty, float rotz){
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
    for(;j<c/3*4;j+=4)
        colorBuffer[j] = (color2.r)/255.0f,
        colorBuffer[j+1] = (color2.g)/255.0f,
        colorBuffer[j+2] = (color2.b)/255.0f,
        colorBuffer[j+3] = alpha,
        
    
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

    for(;j<c/3*4;j+=4)
        colorBuffer[j] = (color1.r)/255.0f,
        colorBuffer[j+1] = (color1.g)/255.0f,
        colorBuffer[j+2] = (color1.b)/255.0f,
        colorBuffer[j+3] = alpha;

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
    
    for(;j<c/3*4;j+=4)
        colorBuffer[j] = (color3.r)/255.0f,
        colorBuffer[j+1] = (color3.g)/255.0f,
        colorBuffer[j+2] = (color3.b)/255.0f,
        colorBuffer[j+3] = alpha;
    
    glm::mat4 rotate = rotation(rotx, roty, rotz);
    for(int i=0;i<c;i+=3) 
    {
        glm::vec4 pos(vertex_buffer_data[i], vertex_buffer_data[i+1],vertex_buffer_data[i+2], 1);
        pos = rotate*pos;
        vertex_buffer_data[i] = pos.x;
        vertex_buffer_data[i+1] = pos.y;
        vertex_buffer_data[i+2] = pos.z;
    }  
    return create3DObject(GL_TRIANGLES, c/3, vertex_buffer_data, colorBuffer, NULL, GL_FILL);
}



struct VAO* GenFrustrum( float x, float y, float z, float r1, float r2, color_t color1, color_t color2, color_t color3, float alpha, float length, int  n, float rotx, float roty, float rotz){
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
    for(;j<c/3*4;j+=4)
        colorBuffer[j] = (color2.r)/255.0f,
        colorBuffer[j+1] = (color2.g)/255.0f,
        colorBuffer[j+2] = (color2.b)/255.0f,
        colorBuffer[j+3] = alpha,
        
    
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

    for(;j<c/3*4;j+=4)
        colorBuffer[j] = (color1.r)/255.0f,
        colorBuffer[j+1] = (color1.g)/255.0f,
        colorBuffer[j+2] = (color1.b)/255.0f,
        colorBuffer[j+3] = alpha;

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
    
    for(;j<c/3*4;j+=4)
        colorBuffer[j] = (color3.r)/255.0f,
        colorBuffer[j+1] = (color3.g)/255.0f,
        colorBuffer[j+2] = (color3.b)/255.0f,
        colorBuffer[j+3] = alpha;
    
    glm::mat4 rotate = rotation(rotx, roty, rotz);
    for(int i=0;i<c;i+=3) 
    {
        glm::vec4 pos(vertex_buffer_data[i], vertex_buffer_data[i+1],vertex_buffer_data[i+2], 1);
        pos = rotate*pos;
        vertex_buffer_data[i] = x+pos.x;
        vertex_buffer_data[i+1] = y+pos.y;
        vertex_buffer_data[i+2] = z+pos.z;
    }  
    return create3DObject(GL_TRIANGLES, c/3, vertex_buffer_data, colorBuffer, NULL, GL_FILL);
}

struct VAO* GenFrustrumHollow( float r1, float r2, float thickness, color_t color1, float alpha, float length, int  n, float ratio, float rotx, float roty, float rotz){
    GLfloat vertex_buffer_data[100000];
    GLfloat colorBuffer[100000];
    int c = 0;

    glm::vec3 pos(r1,0,length/2);
    glm::vec3 posb(r2,0,-length/2);
    glm::vec3 temp = pos;
    float angle = 360/(float)n;

    for(int i=0;i<n*ratio;i++){
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

    pos     = glm::vec3(r1-thickness,0,length/2);
    posb    = glm::vec3(r2-thickness,0,-length/2);
    temp = pos;
    angle = 360/(float)n;

    for(int i=0;i<n*ratio;i++){
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

    pos     = glm::vec3(r1,0,length/2);
    posb    = glm::vec3(r1-thickness,0,length/2);
    temp = pos;
    angle = 360/(float)n;
    for(int i=0;i<n*ratio;i++){
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

    pos     = glm::vec3(r1,0,-length/2);
    posb    = glm::vec3(r1-thickness,0,-length/2);
    temp = pos;
    angle = 360/(float)n;
    for(int i=0;i<n*ratio;i++){
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
    for(;j<c/3*4;j+=4)
        colorBuffer[j] = (color1.r)/255.0f,
        colorBuffer[j+1] = (color1.g)/255.0f,
        colorBuffer[j+2] = (color1.b)/255.0f,
        colorBuffer[j+3] = alpha;
        
    glm::mat4 rotate = rotation(rotx, roty, rotz);
    for(int i=0;i<c;i+=3) 
    {
        glm::vec4 pos(vertex_buffer_data[i], vertex_buffer_data[i+1],vertex_buffer_data[i+2], 1);
        pos = rotate*pos;
        vertex_buffer_data[i] = pos.x;
        vertex_buffer_data[i+1] = pos.y;
        vertex_buffer_data[i+2] = pos.z;
    }  
    return create3DObject(GL_TRIANGLES, c/3, vertex_buffer_data, colorBuffer, NULL, GL_FILL);
}

struct VAO* GenFrustrumHollow( float x, float y, float z, float r1, float r2, float thickness, color_t color1, float alpha, float length, int  n, float ratio, float rotx, float roty, float rotz){
    GLfloat vertex_buffer_data[100000];
    GLfloat colorBuffer[100000];
    int c = 0;

    glm::vec3 pos(r1,0,length/2);
    glm::vec3 posb(r2,0,-length/2);
    glm::vec3 temp = pos;
    float angle = 360/(float)n;

    for(int i=0;i<n*ratio;i++){
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

    pos     = glm::vec3(r1-thickness,0,length/2);
    posb    = glm::vec3(r2-thickness,0,-length/2);
    temp = pos;
    angle = 360/(float)n;

    for(int i=0;i<n*ratio;i++){
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

    pos     = glm::vec3(r1,0,length/2);
    posb    = glm::vec3(r1-thickness,0,length/2);
    temp = pos;
    angle = 360/(float)n;
    for(int i=0;i<n*ratio;i++){
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

    pos     = glm::vec3(r1,0,-length/2);
    posb    = glm::vec3(r1-thickness,0,-length/2);
    temp = pos;
    angle = 360/(float)n;
    for(int i=0;i<n*ratio;i++){
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
    for(;j<c/3*4;j+=4)
        colorBuffer[j] = (color1.r)/255.0f,
        colorBuffer[j+1] = (color1.g)/255.0f,
        colorBuffer[j+2] = (color1.b)/255.0f,
        colorBuffer[j+3] = alpha;
        
    glm::mat4 rotate = rotation(rotx, roty, rotz);
    for(int i=0;i<c;i+=3) 
    {
        glm::vec4 pos(vertex_buffer_data[i], vertex_buffer_data[i+1],vertex_buffer_data[i+2], 1);
        pos = rotate*pos;
        vertex_buffer_data[i] = x+pos.x;
        vertex_buffer_data[i+1] = y+pos.y;
        vertex_buffer_data[i+2] = z+pos.z;
    }  
    return create3DObject(GL_TRIANGLES, c/3, vertex_buffer_data, colorBuffer, NULL, GL_FILL);
}
struct VAO* GenPlaneTextured(float width, float length, float x, float y, float z){
    GLfloat vertex_buffer_data[100000];
    GLfloat colorBuffer[100000];
    GLfloat uvBuffer[100000];
    int c = 0;
    int c2 = 0;

    vertex_buffer_data[c++] = x-width/2;
    vertex_buffer_data[c++] = y-length/2;
    vertex_buffer_data[c++] = z;
    uvBuffer[c2++] = 0;
    uvBuffer[c2++] = 0;

    vertex_buffer_data[c++] = x-width/2;
    vertex_buffer_data[c++] = y+length/2;
    vertex_buffer_data[c++] = z;
    uvBuffer[c2++] = 0;
    uvBuffer[c2++] = 1;

    vertex_buffer_data[c++] = x+width/2;
    vertex_buffer_data[c++] = y+length/2;
    vertex_buffer_data[c++] = z;
    uvBuffer[c2++] = 1;
    uvBuffer[c2++] = 1;

    vertex_buffer_data[c++] = x+width/2;
    vertex_buffer_data[c++] = y+length/2;
    vertex_buffer_data[c++] = z;
    uvBuffer[c2++] = 1;
    uvBuffer[c2++] = 1;

    vertex_buffer_data[c++] = x+width/2;
    vertex_buffer_data[c++] = y-length/2;
    vertex_buffer_data[c++] = z;
    uvBuffer[c2++] = 1;
    uvBuffer[c2++] = 0;

    vertex_buffer_data[c++] = x-width/2;
    vertex_buffer_data[c++] = y-length/2;
    vertex_buffer_data[c++] = z;
    uvBuffer[c2++] = 0;
    uvBuffer[c2++] = 0;

    for(int i=0;i<4*c/3;i++) colorBuffer[i] = 0;

    return create3DObject(GL_TRIANGLES, c/3, vertex_buffer_data, colorBuffer, uvBuffer, GL_FILL);
}

std::pair<float, float> rotate(float angle, float x, float y){
    return std::make_pair(x*cos(angle) - y*sin(angle), x*sin(angle) + y*cos(angle));
}

struct triplet{
    float x;
    float y;
    float z;
};

struct VAO* GenPolygon(float x, float y, float z, float radius, int n, float ratio, color_t color, float alpha, float rotx, float roty, float rotz){
    float vertex_buffer_data[1000000];
    triplet point, center;
	point.x = radius; point.y = point.z = 0;
	center.x = center.y = center.z = 0;
	float theta = 2 * M_PI / (float)n;

	int c = 0;

	for(int t=0;t<=n*ratio;t++){
		vertex_buffer_data[c++] = point.x;
		vertex_buffer_data[c++] = point.y;
		vertex_buffer_data[c++] = point.z;
		
		triplet tmp = point;
		point = center;
		vertex_buffer_data[c++] = point.x;
		vertex_buffer_data[c++] = point.y;
		vertex_buffer_data[c++] = point.z;
		point = tmp;

		std::pair<float, float> tr = rotate(theta, point.x, point.y);
		point.x = tr.first;
		point.y = tr.second;

		vertex_buffer_data[c++] = point.x;
		vertex_buffer_data[c++] = point.y;
		vertex_buffer_data[c++] = point.z;
	}

    glm::mat4 rotate = rotation(rotx, roty, rotz);
    for(int i=0;i<c;i+=3) 
    {
        glm::vec4 pos(vertex_buffer_data[i], vertex_buffer_data[i+1],vertex_buffer_data[i+2], 1);
        pos = rotate*pos;
        vertex_buffer_data[i] = x+pos.x;
        vertex_buffer_data[i+1] = y+pos.y;
        vertex_buffer_data[i+2] = z+pos.z;
    }  
    return create3DObject(GL_TRIANGLES, c/3, vertex_buffer_data, color, alpha, NULL, GL_FILL);
}