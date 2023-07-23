#version 330 core



layout (location=0) in vec4 in_pos;
uniform mat3 vs_transform;



void main(void){
	gl_Position=vec4((vs_transform*vec3(in_pos.xy,1.0)).xy,0.5,1.0);
}
