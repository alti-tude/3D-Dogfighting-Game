#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class FrameBuffer{
private:
    unsigned int rendererID, texture, depthBuffer;
    float width, height;

public:
    FrameBuffer(unsigned int width, unsigned int height);
    FrameBuffer(){}
    ~FrameBuffer(){}
    unsigned int GetTexture(){return texture;}
    unsigned int GetID(){return rendererID;}
    void Bind();
    void UnBind();
};

#endif