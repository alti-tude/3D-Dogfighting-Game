#include "FrameBuffer.h"
#include "main.h"

class Water{
private:
    VAO* obj;
    unsigned int shaderID;
public:
    FrameBuffer* fbo;
    float width, length, sqSize;

public:
    Water(float width, float length, float sqSize);
    ~Water(){}
    void DrawToWater(){ fbo->Bind(); }
    void UnbindFBO() {fbo->UnBind(); }
    void Draw(glm::mat4 VP);
};