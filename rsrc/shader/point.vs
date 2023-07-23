#version 330 core



layout (location=0) in vec4 in_pos;
out float gs_red_color;
uniform mat3 vs_transform;



void main(void){
	gl_Position=vec4((vs_transform*vec3(in_pos.xy,1.0)).xy,0.0,1.0);
	gs_red_color=(in_pos.w==0?0.05:1.0);
}
