#version 330 core 

layout(location = 0) in vec3 inPos;  // Use vec3 for 3D position
layout(location = 1) in vec2 inTex;  // Texture coordinates
out vec2 chTex;

uniform mat4 uM;  // Model matrix for 3D transformations
uniform mat4 uV;   // View matrix for camera
uniform mat4 uP;  // Projection matrix

uniform float p1x;  // Player 1 position
uniform float p2x;  // Player 2 position

void main()
{
    vec4 worldPos = vec4(inPos.x + (inPos.x > 0.0 ? p1x : p2x), inPos.y, inPos.z, 1.0);
    gl_Position = uP*uV*uM*worldPos;

    chTex = inTex;
}
