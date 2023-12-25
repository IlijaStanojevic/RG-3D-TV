#version 330 core

in vec3 fragColor;
out vec4 FragColor;
uniform vec3 pulsatingColor;
uniform vec3 staticColor; 

void main()
{
    FragColor = vec4(pulsatingColor, 1.0);
    // Override the color if the TV is on
    if (staticColor != vec3(0.0))
    {
        FragColor = vec4(staticColor, 1.0);
    }
}
