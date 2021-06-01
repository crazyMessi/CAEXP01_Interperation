#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float light;

void main()
{
    FragColor = texture(ourTexture, TexCoord) ;
    FragColor.x*=light;
    FragColor.y*=light;
    FragColor.z*=light;
    FragColor.w*=light;	
}