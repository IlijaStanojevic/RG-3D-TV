#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inCol;
out vec3 chCol;


uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;


void main()
{
    gl_Position = uP*uM*uV*vec4(inPos, 1.0); 
    chCol = inCol;
}
