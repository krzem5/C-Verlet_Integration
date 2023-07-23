#version 330 core



layout(lines) in;
layout(triangle_strip, max_vertices=4) out;
uniform float gs_half_size;



void main(void){
	vec4 pos_a=gl_in[0].gl_Position;
	vec4 pos_b=gl_in[1].gl_Position;
	vec2 normal=vec2(pos_b.y-pos_a.y,pos_a.x-pos_b.x);
	normal*=gs_half_size/sqrt(normal.x*normal.x+normal.y*normal.y);
	vec4 normal4=vec4(normal,0.0,0.0);
	gl_Position=pos_a-normal4;
	EmitVertex();
	gl_Position=pos_a+normal4;
	EmitVertex();
	gl_Position=pos_b-normal4;
	EmitVertex();
	gl_Position=pos_b+normal4;
	EmitVertex();
	EndPrimitive();
}
