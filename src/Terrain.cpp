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

float stor[900+1][900+1];
float ad[900+1][900+1];
bool bStor[900+1][900+1];
// float slope = 0.5;
float xMax = 0, yMax  = 0;
float vertexBuffer[10000000];
float colorBuffer[10000000];
float waterLine = 0;

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

struct VAO* GenTerrain(int x, int y, float length, vector<pair<int,int>>& v, int a){
    xMax = x;
    yMax = y;
    g_length = length;
    for(int i=0;i<=x;i++)
        for(int j=0;j<=y;j++)
            stor[i][j] = bStor[i][j] = ad[i][j] = 0;

    for(auto it:v){
        for(int i=0;i<=x;i++)
            for(int j=0;j<=y;j++)
                bStor[i][j] = 0;
        float height = ((rand()%500) + 500)/100.0;
        stor[it.first][it.second] = height;
        bStor[it.first][it.second] = 1;
        rec(it.first, it.second, height, (rand()%20+20)/100.0);
    }

    for(int i=0;i<=x;i++)
        for(int j=0;j<=y;j++)
            stor[i][j] = stor[i][j]>=waterLine? stor[i][j]+(rand()%200)/100.0:waterLine,
            bStor[i][j] = 0;
            
    for(int i=1, j=0;i<32;i*=2,j++){
        if( (a&i) == 0 ) continue;
        int x = v[j].first;
        int y = v[j].second;
        int r = 4;
        for(int p = x-5;p<x+5;p++){
            for(int q = y-5;q<y+5;q++){
                if( (p-x)*(p-x)+(q-y)*(q-y) <= r*r)
                    stor[p][q] -=4;
            }
        }
    }

    float left = (float)x/2.0*length*(-1);
    float right = -left;
    float bottom = (float)y/2.0*length*(-1);
    float top = -bottom;

    int c = 0;
    for(int i=0;i<x;i++)
        for(int j=0;j<y;j++){
            
            if(stor[i][j]>=waterLine){
                vertexBuffer[c++] = left+i*length;
                vertexBuffer[c++] = bottom+j*length;
                vertexBuffer[c++] = stor[i][j];
                colorBuffer[c/3*4-4] = 1.0*stor[i][j]/10;
                colorBuffer[c/3*4-3] = 0.4*stor[i][j]/10;
                colorBuffer[c/3*4-1] = 1;
            }
            
            if(stor[i][j]>=waterLine){
                vertexBuffer[c++] = left+i*length,
                vertexBuffer[c++] = bottom+(j+1)*length,
                vertexBuffer[c++] = stor[i][j+1],
                colorBuffer[c/3*4-4] = 1.0*stor[i][j]/10;
                colorBuffer[c/3*4-3] = 0.4*stor[i][j]/10;
                colorBuffer[c/3*4-1] = 1;
            }

            if(stor[i][j]>=waterLine){
                vertexBuffer[c++] = left+(i+1)*length,
                vertexBuffer[c++] = bottom+(j+1)*length,
                vertexBuffer[c++] = stor[i+1][j+1],
                colorBuffer[c/3*4-4] = 1.0*stor[i][j]/10;
                colorBuffer[c/3*4-3] = 0.4*stor[i][j]/10;
                colorBuffer[c/3*4-1] = 1;
            }

            if(stor[i][j]>=waterLine){
                vertexBuffer[c++] = left+i*length,
                vertexBuffer[c++] = bottom+j*length,
                vertexBuffer[c++] = stor[i][j],
                colorBuffer[c/3*4-4] = 1.0*stor[i][j]/10;
                colorBuffer[c/3*4-3] = 0.4*stor[i][j]/10;
                colorBuffer[c/3*4-1] = 1;
            }

            if(stor[i][j]>=waterLine){
                vertexBuffer[c++] = left+(i+1)*length,
                vertexBuffer[c++] = bottom+j*length,
                vertexBuffer[c++] = stor[i+1][j],
                colorBuffer[c/3*4-4] = 1.0*stor[i][j]/10;
                colorBuffer[c/3*4-3] = 0.4*stor[i][j]/10;
                colorBuffer[c/3*4-1] = 1;
            }

            if(stor[i][j]>=waterLine){
                vertexBuffer[c++] = left+(i+1)*length,
                vertexBuffer[c++] = bottom+(j+1)*length,
                vertexBuffer[c++] = stor[i+1][j+1];
                colorBuffer[c/3*4-4] = 1.0*stor[i][j]/10;
                colorBuffer[c/3*4-3] = 0.4*stor[i][j]/10;
                colorBuffer[c/3*4-1] = 1;
            }
        }

    return create3DObject(GL_TRIANGLES, c/3, vertexBuffer,colorBuffer, NULL, GL_FILL);
    
}

void Draw(struct VAO* obj, glm::mat4 VP){
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (glm::vec3(0,0,0));    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(Matrices.modelMatrixID, 1, GL_FALSE, &Matrices.model[0][0]);
    draw3DObject(obj);
}

bool CheckCollisionGround(float x, float y, float z, float buffer){
    int i = (int)( (x+xMax/2)/g_length);
    int j = (int)( (y+yMax/2)/g_length);
    return z<=stor[i][j]+buffer;
}

bool checkVissible(glm::vec3 from, glm::vec3 to){
    // glm::vec3 step = glm::normalize(to-from);
    // while(to!=from) {
    //     from+=step;
    //     cout << from.x << " " << from.y << " " << from.z << endl;
    //     if(CheckCollision(from.x, from.y, from.z, 0.5)) return false;
    // }

    return true;
}