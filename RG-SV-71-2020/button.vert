#version 330 core

layout(location = 0) in vec2 inPos;
out vec2 fragCoord;

void main()
{
    gl_Position = vec4(inPos, 0.0, 1.0);
    fragCoord = inPos;  // Pass the fragment coordinates to the fragment shader
}
