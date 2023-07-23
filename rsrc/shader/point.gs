#version 330 core



layout(points) in;
layout(triangle_strip, max_vertices=4) out;
in float gs_red_color[1];
out float fs_red_color;
uniform float gs_half_size;



void main(void){
	vec4 pos=gl_in[0].gl_Position;
	gl_Position=pos+vec4(-gs_half_size,-gs_half_size,0.0,0.0);
	fs_red_color=gs_red_color[0];
	EmitVertex();
	gl_Position=pos+vec4(gs_half_size,-gs_half_size,0.0,0.0);
	fs_red_color=gs_red_color[0];
	EmitVertex();
	gl_Position=pos+vec4(-gs_half_size,gs_half_size,0.0,0.0);
	fs_red_color=gs_red_color[0];
	EmitVertex();
	gl_Position=pos+vec4(gs_half_size,gs_half_size,0.0,0.0);
	fs_red_color=gs_red_color[0];
	EmitVertex();
	EndPrimitive();
}
