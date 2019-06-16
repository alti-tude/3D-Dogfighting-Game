#version 330 core

in vec4 fragCoords;
in vec4 fragColor;

uniform sampler2D textureSampler;
out vec4 color;

void main(){
    vec4 inverted = fragCoords;
    // inverted.x = -inverted.x;
    vec2 tmp = (-inverted.xy)/(inverted.w*2.0) + 0.5; 
    color = 0.4*fragColor;// + 0.7*texture(textureSampler, tmp);
    // color.x = 1;
    color.r = 0.3;
    color.g = 0.3;
    color.a = 0.7;
}