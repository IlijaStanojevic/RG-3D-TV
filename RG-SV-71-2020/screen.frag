#version 330 core

in vec4 channelCol; // Input color from the vertex shader
out vec4 fragColor; // Output color

uniform vec4 screenColor; // Uniform variable for screen color

void main()
{
    // Print the screen color as RGB values
    vec3 debugColor = screenColor.rgb;
    debugColor = debugColor * 255.0; // Scale to [0, 255] for better visibility
    fragColor = vec4(debugColor, 1.0);
    fragColor = channelCol;
    // Output the original color for visualization
    // fragColor = channelCol;
}
