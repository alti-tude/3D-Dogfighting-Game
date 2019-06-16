#include "main.h"
#include "timer.h"
#include "Terrain.h"
#include "Plane.h"
#include "FrameBuffer.h"
#include "Water.h"
#include "ball.h"
#include "Missiles.h"
#include "Turret.h"
#include "Arrow.h"
#include "Views.h"
#include "DetectCollision.h"
#include "Parachutes.h"
#include "Bar.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;
GLuint     textureActivationID;
GLuint     textureID;

GLMatrices staticMatrices;

/**************************
* Customizable functions *
**************************/
FrameBuffer fbo;
GLfloat gravity = 0.002;
bool scalePlane = 0;

enum stunStates{BARREL_ROLL, LOOP} stuntFlag;
unsigned int stuntTimer;

vector<Missile*> enemyMissiles;
float attackRadius = 100;
unsigned int enemyMissileFreq = 4*60;

Bar* fuelBar;
float fuelRate = 0.5;

Bar* healthBar;
float healthRate = 5;

int score = 0;

struct VAO* smokeRing[5];
glm::vec3 smokeRingPos[5];

vector<Parachute*> parachutes;
unsigned int maxParachutes = 5, parachuteFreq = 2*60;

Plane* plane;
float planeLength=2, planeRadius = planeLength/16, planeCamDist = 1*planeLength;
glm::vec3 planeAcceleration(0,0,0.005);
float planeMaxSpeed = 0.6, planeMaxHeight = 50;

Ball* fuel;

vector<Missile*> missiles;
glm::vec3 missileSpeed(0,0,0.6);
unsigned int missileReload = 60, missilePrev = -60;

vector<Missile*> bombs;
unsigned int bombReload = 1*60, bombPrev = -1*60;

vector<Turret* > turrets;
unsigned int maxTurrets = 4;

Arrow* arrow;
float arrowLength = 20;

struct VAO* terrain;
struct VAO* volcanoCylinder[5];
glm::vec2 volcanoPos[5]; 
Water* water;

struct VAO* plain;
struct VAO* crosshair;
struct VAO* speedometer;
struct VAO* altimeter;
struct speedNeedle{
    struct VAO* needle;
    glm::vec3 pos;
    float rotz;
} speedNeedle;

struct speedNeedle heightNeedle;

vector<struct VAO*> clouds;


enum camStates{FOLLOW, FRONT, TOWER, TOP, HELICOPTER, NONE} camFlag;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

glm::vec3 cam;
glm::vec3 camLook;
glm::vec3 camUp(-1,0,0);
bool freeCam = 0;

void draw() {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram (programID);

    if(camFlag == FOLLOW) 
        planeCam(plane, glm::vec3(0,0,-planeLength), glm::vec3(0,0,planeLength), glm::vec3(-1, 0, 0));
    if(camFlag == FRONT) {
        planeCam(plane, glm::vec3(0,0,planeLength), glm::vec3(0,0,2*planeLength), glm::vec3(-1, 0, 0));
    }
    if(camFlag == TOP) {
        topCam(plane, 15);
    }
    if(camFlag == TOWER) {
        towerCam(plane);
    }
    if(camFlag == HELICOPTER){
        helicopterCam(window, plane);
    }
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Scene render
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glUniform1i(textureActivationID, 0);

        Draw(terrain, VP);        
        plane -> draw(VP, scalePlane);
        plane -> PostDraw();
        // ball1.draw(VP);
        for(auto it:missiles) it->draw(VP);
        for(auto it:enemyMissiles) it->draw(VP);
        for(auto it:bombs) it->draw(VP);
        for(auto it:turrets) it->draw(VP);
        if(fuel!=NULL) fuel->draw(VP);
    glEnable(GL_BLEND);
        arrow->draw(VP);
        for(auto it:parachutes) it->draw(VP);
        for(int i=0;i<5;i++) 
            if(volcanoCylinder[i]!=NULL) Draw(volcanoCylinder[i],VP);
        for(int i=0;i<5;i++) 
            if(smokeRing[i]!=NULL) Draw(smokeRing[i], VP);
    glDisable(GL_DEPTH_TEST);
}

void drawStatic() {

    glUseProgram (programID);

    glm::mat4 VP = staticMatrices.projection * staticMatrices.view;

    // Scene render
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glUniform1i(textureActivationID, 0);
    if(camFlag == FRONT) Draw(crosshair, VP);        
    Draw(speedometer, VP); 
    Draw(altimeter, VP); 
    fuelBar->draw(VP);
    healthBar->draw(VP);

    glm::mat4 rotation = glm::rotate(glm::radians(speedNeedle.rotz), glm::vec3(0,0,-1));
    glm::mat4 t1       = glm::translate(glm::vec3(25, 0, 0));
    glm::mat4 t2       = glm::translate(speedNeedle.pos);
    Draw(speedNeedle.needle, VP*t2*rotation*t1);

    rotation = glm::rotate(glm::radians(heightNeedle.rotz), glm::vec3(0,0,-1));
    t1       = glm::translate(glm::vec3(25, 0, 0));
    t2       = glm::translate(heightNeedle.pos);
    Draw(heightNeedle.needle, VP*t2*rotation*t1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo.GetTexture());
    glUniform1i(textureID, 0);
    glUniform1i(textureActivationID, 1);
        Draw(plain, VP);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
}

void DetectCollisionMissile(){
    glm::vec3 target = turrets[0]->position;
    bounding_box_t targetBox = {target.x,target.y,target.z,2,2,2};
    bounding_box_t missileBox = {0,0,0,0,0,0};

    {
        auto it = missiles.begin();
        while(it!=missiles.end()){
            missileBox = {(*it)->position.x, (*it)->position.y, (*it)->position.z, 0,0,0};
            if(detectCollision(targetBox, missileBox)){
                delete *turrets.begin(), turrets.erase(turrets.begin());
                delete *it, missiles.erase(it);
                glm::vec3 arrowPos = turrets[0]->position;
                score+=10;
                arrow = new Arrow(arrowPos.x, arrowPos.y, arrowPos.z+arrowLength/2+5, 1, arrowLength, {255,0,0});
            }
            else if(CheckCollisionGround((*it)->position.x, (*it)->position.y, (*it)->position.z, planeLength/4) || (*it)->position.z <= 3) 
                delete *it, missiles.erase(it);
            else it++;
        }
    }

    
    {
        auto it2 = parachutes.begin();
        while(it2!=parachutes.end()){
            glm::vec3 target = (*it2)->position;             
            bounding_box_t targetBox = {target.x,target.y,target.z,2,2,2};
            bounding_box_t missileBox = {0,0,0,0,0,0};

            auto it = missiles.begin();
            bool flag = 0;
            while(it!=missiles.end()){
                missileBox = {(*it)->position.x, (*it)->position.y, (*it)->position.z, 0,0,0};
                if(detectCollision(targetBox, missileBox)){
                    delete *it2, parachutes.erase(it2);
                    delete *it, missiles.erase(it);
                    score+=10;
                    flag = 1;
                }
                else it++;
            }
            if(!flag) it2++;
        }
    }
}

void DetectCollisionBomb(){
    glm::vec3 target = turrets[0]->position;
    bounding_box_t targetBox = {target.x,target.y,target.z,2,2,2};
    bounding_box_t bombBox = {0,0,0,0,0,0};

    {
        auto it = bombs.begin();
        while(it!=bombs.end()){
            bombBox = {(*it)->position.x, (*it)->position.y, (*it)->position.z, 0,0,0};
            if(detectCollision(targetBox, bombBox)){
                delete *turrets.begin(), turrets.erase(turrets.begin());
                delete *it, bombs.erase(it);
                glm::vec3 arrowPos = turrets[0]->position;
                score+=10;
                arrow = new Arrow(arrowPos.x, arrowPos.y, arrowPos.z+arrowLength/2+5, 1, arrowLength, {255,0,0});
            }
            else if(CheckCollisionGround((*it)->position.x, (*it)->position.y, (*it)->position.z, planeLength/4) || (*it)->position.z <= 3) 
                delete *it, bombs.erase(it);
            else it++;
        }
    }
}

void DetectCollisionsPlane(){
    glm::vec3 planePos = plane->position;
    vector<glm::vec3 > colPoints = plane->GetColPoints();

    for(auto it:colPoints){
        if(CheckCollisionGround(it.x, it.y, it.z, 0.01) || it.z <= 3) 
            quit(window);
        {
            auto it2 = enemyMissiles.begin();
            while(it2!=enemyMissiles.end()){
                glm::vec3 relTargetPos = (*it2)->position - it;
                if(sqrt(glm::dot(relTargetPos, relTargetPos)) < planeLength/1)
                    delete *it2, enemyMissiles.erase(it2), healthBar->scalar.x -= healthRate;
                else it2++;
            }
        }

        {
            glm::vec3 target = turrets[0]->position;
            bounding_box_t targetBox = {target.x,target.y,target.z,2,2,2};
            bounding_box_t planeBox = {it.x,it.y,it.z,0,0,0};
            if(detectCollision(targetBox, planeBox)){
                delete *turrets.begin(), turrets.erase(turrets.begin());
                glm::vec3 arrowPos = turrets[0]->position;
                healthBar->scalar.x -= 10*healthRate;
                arrow = new Arrow(arrowPos.x, arrowPos.y, arrowPos.z+arrowLength/2+5, 1, arrowLength, {255,0,0});
                score+=10;
            }
        }

        {
            auto it2 = parachutes.begin();
            while(it2!=parachutes.end()){
                glm::vec3 target = (*it2)->position;
                bounding_box_t targetBox = {target.x,target.y,target.z,1.5,1.5,1.5};
                bounding_box_t planeBox = {it.x,it.y,it.z,0,0,0};
                if(detectCollision(targetBox, planeBox)){
                    delete *it2, parachutes.erase(it2);
                    healthBar->scalar.x -= healthRate;
                    score+=10;
                }
                else it2++;
            }
        }

        {
            glm::vec3 target = fuel->position;
            bounding_box_t targetBox = {target.x,target.y,target.z,2,2,2};
            bounding_box_t planeBox = {it.x,it.y,it.z,0,0,0};
            if(detectCollision(targetBox, planeBox)){
                fuelBar->scalar.x += 2*fuelRate;
                healthBar->scalar.x += 0.1*healthRate;
            }
        }

        {
           for(int i=0;i<5;i++){
                if(volcanoCylinder[i]==NULL) continue;
                glm::vec2 relPos(it.x-volcanoPos[i].x, it.y-volcanoPos[i].y);
                if(sqrt(glm::dot(relPos, relPos))<4)
                    healthBar->scalar.x -= 0.1*healthRate;
           }
        }

        {
            for(int i=0;i<5;i++){
                if(smokeRing[i]==NULL) continue;
                glm::vec3 relPos = smokeRingPos[i]-it;
                if(sqrt(glm::dot(relPos, relPos))<=1)
                    smokeRing[i] = NULL, score+=10;
            }
        }
    }
    
}


void ProcessCollisions(){
    DetectCollisionMissile();
    DetectCollisionBomb();
    DetectCollisionsPlane();
}

void BarrelRoll(){
    plane->rotationz = 10;
    stuntTimer--;
}

void loop(){
    stuntTimer--;
    if(fuelBar->scalar.x <= 0) return;
    plane->rotationy = -5;
    if(plane->speed.z < planeMaxSpeed ) 
        plane->speed+=planeAcceleration;
        
    fuelBar->scalar.x -= fuelRate;
    fuelBar->scalar.y = 10;
}

void tick_input(GLFWwindow *window) {
    if(stuntTimer>0){
        if(stuntFlag == BARREL_ROLL) 
            BarrelRoll();
        if(stuntFlag == LOOP)
            loop();
        return;
    }
    int left  = glfwGetKey(window, GLFW_KEY_A);
    int right = glfwGetKey(window, GLFW_KEY_D);
    int fwd = glfwGetKey(window, GLFW_KEY_W);
    int bwd = glfwGetKey(window, GLFW_KEY_S);
    int rollL = glfwGetKey(window, GLFW_KEY_Q);
    int rollR = glfwGetKey(window, GLFW_KEY_E);
    int accel = glfwGetKey(window, GLFW_KEY_SPACE);

    int followCam = glfwGetKey(window, GLFW_KEY_1);
    int frontCam = glfwGetKey(window, GLFW_KEY_2);
    int topCam = glfwGetKey(window, GLFW_KEY_3);
    int towerCam = glfwGetKey(window, GLFW_KEY_4);
    int helicopterCam = glfwGetKey(window, GLFW_KEY_5);
    int dc = glfwGetKey(window, GLFW_KEY_X);

    int barrelRoll = glfwGetKey(window, GLFW_KEY_B);
    int loop = glfwGetKey(window, GLFW_KEY_L);
    int shootM = glfwGetKey(window, GLFW_KEY_M);
    int shootB = glfwGetKey(window, GLFW_KEY_N);

    if(barrelRoll) stuntFlag = BARREL_ROLL, stuntTimer = 36;
    if(loop) stuntFlag = LOOP, stuntTimer = 70;

    if(shootM && t60.frameCount - missilePrev>=missileReload){
        missiles.push_back(new Missile(plane->position.x, plane->position.y, plane->position.z-3*planeRadius, planeRadius/2, planeLength, {255, 100, 150}));
        missiles[missiles.size()-1]->position = plane->TransformAsPlane(glm::vec3(planeRadius*3, 0, planeLength/4));
        missiles[missiles.size()-1]->speed = plane->TransformAsPlane(glm::vec3(0,0,1))-plane->position;        
        missilePrev = t60.frameCount;
    }
    if(shootB && t60.frameCount - bombPrev>=bombReload){
        bombs.push_back(new Missile(plane->position.x, plane->position.y, plane->position.z-3*planeRadius, planeRadius/2, planeLength/4, {255, 150, 100}));
        bombs[bombs.size()-1]->windUp = 0;
        bombs[bombs.size()-1]->position = plane->TransformAsPlane(glm::vec3(planeRadius*3, 0, 0));        
        bombPrev = t60.frameCount;
    }

    if(accel && fuelBar->scalar.x > 0) {
        if(plane->speed.z < planeMaxSpeed ) 
            plane->speed+=planeAcceleration;
        
        fuelBar->scalar.x -= fuelRate;
        fuelBar->scalar.y = 10;
    }
    else if(plane->speed.z > 0.001) {
        plane->speed -= planeAcceleration;
    }
    else plane->speed = glm::vec3(0,0,0);

    if(rollL){
        plane->rotationz =-1;
    }
    if(rollR){
        plane->rotationz =1;
    }
    if (left) {
        plane->rotationx =-1;
    }
    if(right) {
        plane->rotationx =1;
    }
    if(fwd){
        // camx += 0.1;
        plane->rotationy = -1;
    }
    if(bwd){
        // camx -= 0.1;
        plane->rotationy = 1;
    }

    if(followCam) camFlag = FOLLOW;
    if(frontCam) camFlag = FRONT;
    if(topCam) camFlag = TOP;
    if(towerCam) camFlag = TOWER;
    if(helicopterCam) camFlag = HELICOPTER;

}

void tick_elements() {
    plane -> tick();
    plane->adSpeed.z -= gravity;

    {
        auto it = missiles.begin();
        while(it!=missiles.end()) {
            (*it)->speed.z -= gravity;
            (*it)->tick();
            if((*it)->windUp>0){
                (*it)->position = plane->TransformAsPlane(glm::vec3(planeRadius*3, 0, 0));
                (*it)->speed = plane->TransformAsPlane(glm::vec3(0,0,1))-plane->position;
            }
            else if((*it)->windUp == 0){
                if(camFlag == FRONT) (*it)->position = plane->TransformAsPlane(glm::vec3(0,0,planeLength));
                (*it)->speed =plane->TransformAsPlane(missileSpeed) - plane->position+plane->TransformAsPlane(plane->speed) - plane->position;
            }
            it++;
            
        }
    }

    {
        auto it = enemyMissiles.begin();
        while(it!=enemyMissiles.end()) {
            // (*it)->speed.z -= gravity;
            (*it)->tick();
            if(CheckCollisionGround((*it)->position.x, (*it)->position.y,(*it)->position.z,0.5) || abs((*it)->position.x)>100 || abs((*it)->position.y)>100 || (*it)->position.z>50)
                delete *it, enemyMissiles.erase(it);
            else it++;
        }
    }

    for(auto it:bombs){
        if(it->windUp==0) {
            if(camFlag == FRONT) it->position = plane->position;
            it->speed = plane->TransformAsPlane(plane->speed) - plane->position;
            // it->speed.x /=2;
            // it->speed.y /=2;
            // it->speed.z /=2;
        }
        it->speed.z -= gravity;
        it->tick();
    }
    {
        auto it = parachutes.begin();
        while(it!=parachutes.end()){
            (*it)->speed.z-=gravity/10;
            (*it)->tick();
            if((*it)->position.z <=2)
                delete *it, parachutes.erase(it);
            else it++;
        }
    }

    fuelBar->scalar.x = max(fuelBar->scalar.x, 0.0f);
    fuelBar->scalar.x = min(fuelBar->scalar.x, fuelBar->ma);
    if(fuelBar->scalar.x == 0) healthBar->scalar.x -= 2*healthRate;

    healthBar->scalar.x = min(healthBar->scalar.x, healthBar->ma);
    healthBar->scalar.x = max(healthBar->scalar.x, 0.0f);
    plane->position.z = min(plane->position.z, planeMaxHeight);
    heightNeedle.rotz = 180* (plane->position.z-3)/planeMaxHeight;
    speedNeedle.rotz = 180*plane->speed.z/planeMaxSpeed;

    if(healthBar->scalar.x ==0) quit(window);
}

void GenerateRandom(){
    if(parachutes.size()<maxParachutes && t60.frameCount%parachuteFreq==0) 
       parachutes.push_back(new Parachute( (rand()%2000)/10-100, (rand()%2000)/10-100, (rand()%400)/10+40, COLOR_GREEN, 1.5));
    
    if(sqrt(glm::dot(plane->position- turrets[0]->position, plane->position- turrets[0]->position))<=attackRadius && enemyMissiles.size()<10 && t60.frameCount%enemyMissileFreq == 0) {
        glm::vec3 turretPos = turrets[0]->position;
        enemyMissiles.push_back(new Missile(turretPos.x, turretPos.y, turretPos.z, 0.5, 2, {178, 239, 112}));
        glm::vec3 speed = glm::normalize(plane->position - turretPos);
        speed.x *= missileSpeed.z;
        speed.y *= missileSpeed.z;
        speed.z *= missileSpeed.z;
        enemyMissiles[enemyMissiles.size()-1] -> speed = speed;
        enemyMissiles[enemyMissiles.size()-1] -> windUp = 0;
    }
}

void CheckPointsCleared(){
    vector<pair<int,int> > v;
    for(int i=0;i<5;i++) v.push_back(make_pair(rand()%180+10, rand()%180+10));
    int a = rand()%32;
    terrain     = GenTerrain(200, 200, 1, v, a);
    for(int i=1, j=0;i<32;i*=2, j++){
        if( (i&a)==0 ) volcanoCylinder[j] = NULL;
        else {
            volcanoCylinder[j] = GenFrustrum(-100.0f + v[j].first, -100.0f+v[j].second, 10, 4, 4, {175, 147, 107}, {175, 147, 107}, {175, 147, 107}, 0.3, 100, 360, 0,0,0);
            volcanoPos[j] = glm::vec2(-100.0f + v[j].first, -100.0f+v[j].second);
        }
    }
    turrets.clear();
    for(int i=0;i<maxTurrets;i++){
        glm::vec3 turretPos((rand()%20000)/100.0-100, (rand()%20000)/100.0-100, 3);
        while(CheckCollisionGround(turretPos.x, turretPos.y, turretPos.z, 1) || turretPos.z < 3.5){
            turretPos.z+=0.5;
        }
        turrets.push_back(new Turret(turretPos.x, turretPos.y, turretPos.z, {19, 130, 5}));
    }

    for(int i=0;i<5;i++){
        glm::vec3 ringPos((rand()%20000)/100.0-100, (rand()%20000)/100.0-100, (rand()%300)/10+15);
        smokeRing[i] = GenFrustrumHollow(ringPos.x, ringPos.y, ringPos.z, 2.0f, 2.0f, 0.1f, {100,100,100}, 0.4, 0.25, 360, 1, rand()%90, rand()%80+10,0);
        smokeRingPos[i] = ringPos;
    }

    glm::vec3 arrowPos = turrets[0]->position;
    arrow = new Arrow(arrowPos.x, arrowPos.y, arrowPos.z+arrowLength/2 + 5, 1, arrowLength, {255,0,0});
    fuel = new Ball( (rand()%1800)/10.0 - 90.0, (rand()%1800)/10.0 - 90.0, (rand()%25)+20, {255,0,0} ); 
    healthBar->scalar.x += 100;
}

void initGL(GLFWwindow *window, int width, int height) {
    fuel        = NULL;
    crosshair   = GenFrustrumHollow(10, 10, 5, COLOR_BLACK, 1, 50, 360, 1, 0, 0, 0);
    plane       = new Plane(0, 0, 10, planeRadius, planeLength, COLOR_RED);
    plane->speed = plane->TransformAsPlane(glm::vec3(0,0,0))-plane->position;

    
    plain       = GenPlaneTextured(152,114, 320,-240, 0);
    speedometer = GenPolygon(-320, 240, 0, 57, 720, 0.5, COLOR_GREEN, 1, 0, 0, 180);
    speedNeedle.needle = GenFrustrum(5, 5, COLOR_BLACK, COLOR_RED, COLOR_BLACK, 1, 50, 4, 0, 90, 45);
    speedNeedle.pos = glm::vec3(-320, 240, 0);
    speedNeedle.rotz = 0;

    altimeter = GenPolygon(320, 240, 0, 57, 720, 0.5, COLOR_BLACK, 1, 0, 0, 180);
    heightNeedle.needle = GenFrustrum(5, 5, COLOR_BLACK, COLOR_RED, COLOR_BLACK, 1, 50, 4, 0, 90, 45);
    heightNeedle.pos = glm::vec3(320, 240, 0);
    heightNeedle.rotz = 0;

    water       = new Water(200, 200, 1);
    cam = glm::vec3(0,0,-planeCamDist);
    camLook = glm::vec3(0,0,planeCamDist);

    fuelBar = new Bar(0,270,0, 0,0,0, COLOR_RED, 360);
    fuelBar->scalar = glm::vec3(360, 10, 1);

    healthBar = new Bar(0,220,0, 0,0,0, {85, 139, 168}, 250);
    healthBar->scalar = glm::vec3(250, 10, 1);

    CheckPointsCleared();
    // Create and compile ouDisablr GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
    Matrices.modelMatrixID = glGetUniformLocation(programID, "M");
    textureActivationID = glGetUniformLocation(programID, "textureActivated");
    textureID= glGetUniformLocation(programID, "textureSampler");

    reshapeWindow (window, width, height);

    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glDisable(GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 800;
    int height = 600;

    // FrameBuffer fbo(800, 600);
    window = initGLFW(width, height);

    initGL (window, width, height);
    fbo = FrameBuffer(800,600);
    fbo.UnBind();
    while (!glfwWindowShouldClose(window)) {

        if (t60.processTick()) {
            fbo.Bind();
            topCam(plane, 70);
            camStates tmp = camFlag;
            camFlag = NONE;
            scalePlane = 1;
            draw();
            fbo.UnBind(); 
            camFlag = tmp;
            scalePlane = 0;
            // water->DrawToWater();
            // glEnable(GL_CLIP_DISTANCE0);
            // drawInverted();
            // glDisable(GL_CLIP_DISTANCE0);

            // water->UnbindFBO();
            
            draw();
            glEnable(GL_DEPTH_TEST);
            water->Draw(Matrices.projection*Matrices.view);

            // glUseProgram(programID);
            //

            drawStatic();
            glfwSwapBuffers(window);
            
            tick_elements();
            ProcessCollisions();
            if(turrets.size() == 0) CheckPointsCleared(); 

            tick_input(window);
            GenerateRandom();
        }

        glfwPollEvents();
    }

    quit(window);
}



void reset_screen() {
    float top    = screen_center_y + 300;
    float bottom = screen_center_y - 300;
    float left   = screen_center_x - 400;
    float right  = screen_center_x + 400;
    staticMatrices.projection = glm::ortho(left, right, top, bottom, 0.1f, 500.0f);
    staticMatrices.view = glm::lookAt(glm::vec3(0,0,-10), glm::vec3(0,0,0), glm::vec3(0,1,0));
    Matrices.projection = glm::perspective(glm::radians(60.0f), 4.0f/3.0f, 0.1f, 500.0f);
}


// void drawInverted() {
//     glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     glUseProgram (programID);

//     if(!freeCam){
//         glm::vec3 eye = plane->TransformAsPlane(cam);
//         glm::vec3 target = plane->TransformAsPlane(camLook);//(plane->position.x, plane->position.y, plane->position.z);
//         target = (target - eye);  
//         target.z = -target.z;      
//         eye.z = 4-(eye.z-4);
//         target+=eye;
//         // target.z = eye.z;
//         glm::vec3 up = -plane->TransformAsPlane(camUp) + plane->position;
//         // up.z = -up.z;
//         cout << up.z << " " << up.y << " " << up.x << endl;
//         Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
//         // Matrices.view *= glm::scale(glm::vec3(-1, 1,1));
//     }
//     else{
//         glm::vec3 eye = glm::vec3(0,0,30);
//         glm::vec3 target = glm::vec3(0,0,0);//(plane->position.x, plane->position.y, plane->position.z);
        
//         glm::vec3 up = glm::vec3(0,1,0);
//         Matrices.view = glm::lookAt( eye, target, up ); 
//     }
//     // float top    = screen_center_y + 300; top/=10;
//     // float bottom = screen_center_y - 300; bottom/=10;
//     // float left   = screen_center_x - 400; left/=10;
//     // float right  = screen_center_x + 400; right/=10;
//     // staticMatrices.projection = glm::ortho(left, right, top, bottom, 0.1f, 500.0f);
//     glm::mat4 VP = Matrices.projection * Matrices.view;

//     // Scene render
//     glDisable(GL_BLEND);
//     glEnable(GL_DEPTH_TEST);
//     glUniform1i(textureActivationID, 0);

//         Draw(terrain, VP);        
//         plane -> draw(VP);
//         plane -> PostDraw();
//         ball1.draw(VP);

//     glEnable(GL_BLEND);
//     glDisable(GL_DEPTH_TEST);
// }