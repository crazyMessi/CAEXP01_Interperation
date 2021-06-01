#version 330 core
out vec4 FragColor;

in vec2 mouth_tex;
in vec2 mask_tex;

uniform float status;
uniform sampler2D texture1;
uniform sampler2D texture2;



void main(){
    
    vec4 mouth_Color = texture(texture1,mouth_tex) ;
    vec4 mask_Color = texture(texture2,mask_tex) ;
    FragColor = vec4(mouth_Color + (mask_Color-mouth_Color)*status) ;
}