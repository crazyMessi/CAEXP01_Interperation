#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec2 offset;

uniform int status;
uniform int times;
out vec2 TexCoord;

void main()
{
    		
    gl_Position = vec4(aPos, 1.0);
    TexCoord = vec2(tex);
    gl_Position.x = gl_Position.x + offset.y*status*times;
    TexCoord.x = TexCoord.x + offset.x*status*times;

}