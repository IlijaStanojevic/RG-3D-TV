#version 330 core

in vec4 channelCol; 
out vec4 fragColor; 
uniform int state; 



void main()
{
    fragColor = channelCol;
    if (state == 0){
        fragColor = vec4(1.0, 0.0, 0.0, 0.0);
    }
    else if (state == 1){
        fragColor = vec4(0.0, 1.0, 0.0, 0.0);
    }else{
        fragColor = vec4(1.0, 1.0, 0.0, 0.0);
    }
}
