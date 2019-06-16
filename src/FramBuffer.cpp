#include "FrameBuffer.h"
#include "main.h"

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height){
    this->width = width;
    this->height = height;
    glGenFramebuffers(1, &this->rendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, this->rendererID);
    

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    this->texture = textureID;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureID, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};     
    glDrawBuffers(1, drawBuffers);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) std::cout << "err\n";

    unsigned int depthBufferID;
    glGenRenderbuffers(1, &depthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
    this->depthBuffer = depthBufferID;
}

void FrameBuffer::Bind(){
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, this->rendererID);
    glViewport(0,0, this->width, this->height);
}

void FrameBuffer::UnBind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,800, 600);
}