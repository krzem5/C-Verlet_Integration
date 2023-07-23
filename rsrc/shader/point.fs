#version 330 core



in float fs_red_color;
out vec4 out_color;



void main(void){
	out_color=vec4(fs_red_color,0.05,0.05,1.0);
}
