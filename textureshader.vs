layout (location = 0) in vec2 mouth_Pos;
layout (location = 1) in vec3 mouth_color;
layout (location = 2) in vec2 mask_Pos;
layout (location = 3) in vec3 mask_color;


uniform float status;
out vec2 mouth_tex;
out vec2 mask_tex;


void main(){
	
	//◊Ï¥ΩŒ∆¿Ì
	mouth_tex = vec2(mouth_Pos.x*0.8+0.5,-1*mouth_Pos.y*0.95+0.50);

	//ø⁄’÷Œ∆¿Ì
	mask_tex = vec2(mask_Pos.x*0.36+0.5,-1*mask_Pos.y*0.43+0.45);

	gl_Position = vec4(mouth_Pos+(mask_Pos-mouth_Pos)*status, 0.0,1.0);	
}