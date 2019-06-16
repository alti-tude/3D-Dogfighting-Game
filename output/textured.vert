#version 330 core

// input data : sent from main program
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec2 vertexUV;
uniform mat4 MVP;

// output data : used by fragment shader
out vec4 fragColor;
out vec4 fragCoords;

void main ()
{
    vec4 v = vec4(vertexPosition, 1); // Transform an homogeneous 4D vector
    fragColor = vertexColor;
    fragCoords = MVP * v;
    // Output position of the vertex, in clip space : MVP * position
    gl_Position = fragCoords;
}
