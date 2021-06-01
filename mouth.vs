#version 330 core
layout (location = 0) in vec2 mouth_Pos;
layout (location = 1) in vec3 mouth_color;
layout (location = 2) in vec2 mask_Pos;
layout (location = 3) in vec3 mask_color;


uniform float status;
out vec3 f_color;



void main()
{
    
    gl_Position = vec4(mouth_Pos+(mask_Pos-mouth_Pos)*status, 0.0,1.0);
    f_color = vec3(mouth_color+(mask_color-mouth_color)*status);

}