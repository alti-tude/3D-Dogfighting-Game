#include "main.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <utility>
#include <queue>
using namespace std;

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

float stor[1000+1][1000+1];
float ad[1000+1][1000+1];
bool bStor[1000+1][1000+1];
// float slope = 0.5;
float xMax = 0, yMax  = 0;
float vertexBuffer[10000000];
float colorBuffer[10000000];

float g_length;

void rec(int x, int y, float st, float slope){
    queue<pair<int, int> > q;
    q.push(make_pair(x,y));
    ad[x][y] = st-slope;

    while(!q.empty()){
        pair<int, int> cur = q.front();
        q.pop();
        int x=cur.first;
        int y=cur.second;
        stor[x][y] < 0.5? stor[x][y] = 0:stor[x][y] *=1;
        st = ad[x][y];
        bStor[x][y] = 1;
        if(x+1<=xMax && !bStor[x+1][y]) 
            q.push(make_pair(x+1, y)), bStor[x+1][y] = 1, stor[x+1][y]=stor[x+1][y]<st? st: stor[x+1][y], ad[x+1][y] = st-slope;
        if(y+1<=yMax && !bStor[x][y+1]) 
            q.push(make_pair(x, y+1)), bStor[x][y+1] = 1, stor[x][y+1]=stor[x][y+1]<st? st: stor[x][y+1], ad[x][y+1] = st-slope;
        if(x-1>=0 && !bStor[x-1][y]) 
            q.push(make_pair(x-1, y)), bStor[x-1][y] = 1, stor[x-1][y]=stor[x-1][y]<st? st: stor[x-1][y], ad[x-1][y] = st-slope;
        if(y-1>=0 && !bStor[x][y-1]) 
            q.push(make_pair(x, y-1)), bStor[x][y-1] = 1, stor[x][y-1]=stor[x][y-1]<st? st: stor[x][y-1], ad[x][y-1] = st-slope;
    }
}

struct VAO* GenTerrain(int x, int y, float length){
    // struct VAO* obj = new struct VAO;

    xMax = x;
    yMax = y;
    g_length = length;

    stor[x/2][x/2] = 10;
    bStor[x/2][y/2] = 1;
    rec(x/2, y/2, 10, 0.3);

    for(int i=0;i<=x;i++)
        for(int j=0;j<=y;j++)
            bStor[i][j] = 0;

    stor[x/2+20][y/2] = 10;
    bStor[x/2+20][y/2] = 1;
    rec(x/2+20, y/2, 10, 0.5);

    for(int i=0;i<=x;i++)
        for(int j=0;j<=y;j++)
            stor[i][j] += (rand()%200)/100.0,
            bStor[i][j] = 0;

    float left = (float)x/2.0*length*(-1);
    float right = -left;
    float bottom = (float)y/2.0*length*(-1);
    float top = -bottom;

    int c = 0;
    std::cout << "Here\n" << std::endl;
    for(int i=0;i<x;i++)
        for(int j=0;j<y;j++){
            vertexBuffer[c++] = left+i*length,
            vertexBuffer[c++] = bottom+j*length,
            vertexBuffer[c++] = stor[i][j],
            colorBuffer[c-2] = 1.0*stor[i][j]/10;
            colorBuffer[c-1] = 1.0-1.0*stor[i][j]/10;

            vertexBuffer[c++] = left+i*length,
            vertexBuffer[c++] = bottom+(j+1)*length,
            vertexBuffer[c++] = stor[i][j+1],
            colorBuffer[c-2] = 1.0*stor[i][j]/10;
            colorBuffer[c-1] = 1.0-1.0*stor[i][j]/10;

            vertexBuffer[c++] = left+(i+1)*length,
            vertexBuffer[c++] = bottom+(j+1)*length,
            vertexBuffer[c++] = stor[i+1][j+1],
            colorBuffer[c-2] = 1.0*stor[i][j]/10;
            colorBuffer[c-1] = 1.0-1.0*stor[i][j]/10;

            vertexBuffer[c++] = left+i*length,
            vertexBuffer[c++] = bottom+j*length,
            vertexBuffer[c++] = stor[i][j],
            colorBuffer[c-2] = 1.0*stor[i][j]/10;
            colorBuffer[c-1] = 1.0-1.0*stor[i][j]/10;

            vertexBuffer[c++] = left+(i+1)*length,
            vertexBuffer[c++] = bottom+j*length,
            vertexBuffer[c++] = stor[i+1][j],
            colorBuffer[c-2] = 1.0*stor[i][j]/10;
            colorBuffer[c-1] = 1.0-1.0*stor[i][j]/10;

            vertexBuffer[c++] = left+(i+1)*length,
            vertexBuffer[c++] = bottom+(j+1)*length,
            vertexBuffer[c++] = stor[i+1][j+1];
            colorBuffer[c-2] = 1.0*stor[i][j]/10;
            colorBuffer[c-1] = 1.0-1.0*stor[i][j]/10;

        }
    
    std::cout << c << "\n";
    // for(int i=0;i<c;i++) std::cout << vertexBuffer[i] << ((i+1)%3!=0? " ":"\n");
    return create3DObject(GL_TRIANGLES, c/3, vertexBuffer,colorBuffer, GL_FILL);
    
}

void draw(struct VAO* obj, glm::mat4 VP){
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (glm::vec3(0,0,0));    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(obj);
}

bool CheckCollision(float x, float y, float z, float buffer){
    int i = (int)(x/g_length);
    int j = (int)(y/g_length);
    return z+buffer<=stor[i][j];
}