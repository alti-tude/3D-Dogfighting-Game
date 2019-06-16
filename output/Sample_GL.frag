#version 330 core

// Interpolated values from the vertex shaders
in vec4 fragColor;
in vec2 uv;

uniform sampler2D textureSampler;
uniform int textureActivated;
// output data
layout(location = 0) out vec4 color;
void main()
{
    color = (1-textureActivated)*fragColor + textureActivated*texture(textureSampler, uv);
    // color.a = 1;
}
