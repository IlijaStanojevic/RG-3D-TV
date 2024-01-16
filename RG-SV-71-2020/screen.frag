#version 330 core

in vec4 channelCol; 
out vec4 fragColor; 
uniform int isTvOn; 



void main()
{
    fragColor = channelCol;
    if (isTvOn == 1){
        fragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
    else{
        fragColor = vec4(1.0, 1.0, 1.0, 0.0);
    }
    fragColor = channelCol;
}
