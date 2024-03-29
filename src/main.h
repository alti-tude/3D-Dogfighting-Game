#ifndef MAIN_H
#define MAIN_H
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <utility>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct color_t {
    int r;
    int g;
    int b;
};

//Shapes.cpp
struct VAO* GenFrustrum( float r1, float r2, color_t color1, color_t color2, color_t color3, float alpha, float length, int  n, float rotx, float roty, float rotz);
struct VAO* GenPlaneTextured(float width, float length, float x, float y, float z);
struct VAO*GenPolygon(float x, float y, float z, float radius, int n, float ratio, color_t color, float alpha, float rotx, float roty, float rotz);
struct VAO* GenFrustrumHollow( float r1, float r2, float thickness, color_t color1, float alpha, float length, int  n, float ratio, float rotx, float roty, float rotz);
struct VAO* GenFrustrumHollow( float x, float y, float z, float r1, float r2, float thickness, color_t color1, float alpha, float length, int  n, float ratio, float rotx, float roty, float rotz);
struct VAO* GenFrustrum( float x, float y, float z, float r1, float r2, color_t color1, color_t color2, color_t color3, float alpha, float length, int  n, float rotx, float roty, float rotz);
// nonedit.cpp
GLFWwindow *initGLFW(int width, int height);
GLuint     LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat *color_buffer_data,  const GLfloat *uv_buffer_data, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha, const GLfloat *uv_buffer_data, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const color_t color, const GLfloat alpha, const GLfloat *uv_buffer_data, GLenum fill_mode = GL_FILL);
void       draw3DObject(struct VAO *vao);

// input.cpp
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
void keyboardChar(GLFWwindow *window, unsigned int key);
void mouseButton(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
extern int yOffset;
// other_handlers.cpp
void error_callback(int error, const char *description);
void quit(GLFWwindow *window);
void reshapeWindow(GLFWwindow *window, int width, int height);

// Types
struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;
    GLuint uvBuffer;
    GLenum PrimitiveMode;
    GLenum FillMode;
    int    NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint    MatrixID;
    GLuint   modelMatrixID;
};

extern GLMatrices Matrices;
extern int score;
// ---- Logic ----

enum direction_t { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };

struct bounding_box_t {
    float x;
    float y;
    float z;
    float xl;
    float zl;
    float yl;
};


extern float screen_zoom, screen_center_x, screen_center_y;
void reset_screen();

// ---- Colors ----
extern const color_t COLOR_RED;
extern const color_t COLOR_GREEN;
extern const color_t COLOR_BLACK;
extern const color_t COLOR_BACKGROUND;

#endif
