#version 330 core

in vec4 channelCol; 
out vec4 fragColor; 

uniform int isTvOn; 

void main()
{
    fragColor = channelCol;
}
