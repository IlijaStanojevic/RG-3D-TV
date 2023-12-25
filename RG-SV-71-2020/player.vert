#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex; // Koordinate texture, propustamo ih u FS kao boje
out vec2 chTex;

uniform float p1x; // player 1 position
uniform float p2x; // player 2 position

void main()
{
    // Adjust the position based on p1x or p2x depending on x
    gl_Position = vec4(inPos.x + (inPos.x > 0.0 ? p1x : p2x), inPos.y, 0.0, 1.0);

    chTex = inTex;
}
