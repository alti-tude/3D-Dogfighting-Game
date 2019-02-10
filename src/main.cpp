#include "main.h"
#include "timer.h"
#include "ball.h"
#include "Terrain.h"
#include "Plane.h"

// #include "DetectCollision.cpp"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;
Plane* plane;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
struct VAO* terrain;

Timer t60(1.0 / 60);

glm::vec3 cam;
glm::vec3 camLook;
glm::vec3 camUp(-1,0,0);
bool freeCam = 0;

void draw() {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram (programID);

    // cam = plane->GetDir();
    // cam.x = 2*plane->position.x - cam.x;
    // cam.y = 2*plane->position.y - cam.y;
    // cam.z = 2*plane->position.z - cam.z;

    if(!freeCam){
        glm::vec3 eye = plane->TransformAsPlane(cam);
        glm::vec3 target = plane->TransformAsPlane(camLook);//(plane->position.x, plane->position.y, plane->position.z);
        glm::vec3 up = plane->TransformAsPlane(camUp) - plane->position;
        Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    }
    else{
        glm::vec3 eye = glm::vec3(0,0,30);
        glm::vec3 target = glm::vec3(0,0,0);//(plane->position.x, plane->position.y, plane->position.z);
        glm::vec3 up = glm::vec3(0,1,0);
        // Compute Camera matrix (view)
        Matrices.view = glm::lookAt( eye, target, up ); 
    }
    glm::mat4 VP = Matrices.projection * Matrices.view;

    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    plane -> draw(VP);
    draw(terrain, VP);
    plane -> PostDraw();
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_A);
    int right = glfwGetKey(window, GLFW_KEY_D);
    int fwd = glfwGetKey(window, GLFW_KEY_W);
    int bwd = glfwGetKey(window, GLFW_KEY_S);
    int rollL = glfwGetKey(window, GLFW_KEY_Q);
    int rollR = glfwGetKey(window, GLFW_KEY_E);
    int ac = glfwGetKey(window, GLFW_KEY_SPACE);
    int dc = glfwGetKey(window, GLFW_KEY_X);

    if(ac) {
        plane->speed = plane->TransformAsPlane(glm::vec3(0,0,0.1))-plane->position;
    }
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
        plane->rotationy =1;
    }
    if(bwd){
        // camx -= 0.1;
        plane->rotationy = -1;
        freeCam = 0;
    }
    if(dc) {
        freeCam = 1;
    }

}

void tick_elements() {
    camera_rotation_angle += 1;
    plane -> tick();
    plane->speed = glm::vec3(0,0,0);
}

void initGL(GLFWwindow *window, int width, int height) {

    // ball1       = Ball(10, 10, COLOR_BLACK);
    // ball1.speed = -0.1;
    plane       = new Plane(0, 0, 20, 0.5, 4, COLOR_RED);
    terrain     = GenTerrain(500, 500, 1);
    cam.x = 0;
    cam.y = 0;
    cam.z = -5;
    camLook.x = 0;
    camLook.y = 0;
    camLook.z = 5;
    // plane -> speed.x = 0.1;
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
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

    window = initGLFW(width, height);

    initGL (window, width, height);

    while (!glfwWindowShouldClose(window)) {

        if (t60.processTick()) {
            draw();
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        glfwPollEvents();
    }

    quit(window);
}



void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::perspective(glm::radians(60.0f), 4.0f/3.0f, 0.1f, 500.0f);
}
