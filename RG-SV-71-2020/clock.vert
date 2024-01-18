#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inCol;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

uniform vec2 rotationCenter; 
uniform float rotationAngle;

out vec4 channelCol;

void main()
{
    // Translate the line to the rotation center
    vec2 translatedPos = inPos.xy - rotationCenter;


    float c = cos(rotationAngle);
    float s = sin(rotationAngle);
    mat2 rotationMatrix = mat2(c, -s, s, c);
    vec2 rotatedPos = rotationMatrix * translatedPos;

    vec3 finalPos = vec3(rotatedPos + rotationCenter, inPos.z);

    gl_Position = uP * uV * uM * vec4(finalPos, 1.0);
    channelCol = inCol;
}
