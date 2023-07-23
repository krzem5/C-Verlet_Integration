#include <verlet_integration/generated/shaders.h>



unsigned int _compiled_programs[2];
unsigned int _program_uniforms[4];



const shader_program_source_uniform_t _shader_uniforms_line[2]={
	{
		"vs_transform",
		0
	},
	{
		"gs_half_size",
		1
	},
};



const shader_program_source_uniform_t _shader_uniforms_point[2]={
	{
		"vs_transform",
		2
	},
	{
		"gs_half_size",
		3
	},
};



const shader_program_source_t _program_sources[2]={
	{
		"#version 330 core\n\n\n\nlayout (location=0) in vec4 in_pos;\nuniform mat3 vs_transform;\n\n\n\nvoid main(void){\n\tgl_Position=vec4((vs_transform*vec3(in_pos.xy,1.0)).xy,0.5,1.0);\n}\n",
		"#version 330 core\n\n\n\nlayout(lines) in;\nlayout(triangle_strip, max_vertices=4) out;\nuniform float gs_half_size;\n\n\n\nvoid main(void){\n\tvec4 pos_a=gl_in[0].gl_Position;\n\tvec4 pos_b=gl_in[1].gl_Position;\n\tvec2 normal=vec2(pos_b.y-pos_a.y,pos_a.x-pos_b.x);\n\tnormal*=gs_half_size/sqrt(normal.x*normal.x+normal.y*normal.y);\n\tvec4 normal4=vec4(normal,0.0,0.0);\n\tgl_Position=pos_a-normal4;\n\tEmitVertex();\n\tgl_Position=pos_a+normal4;\n\tEmitVertex();\n\tgl_Position=pos_b-normal4;\n\tEmitVertex();\n\tgl_Position=pos_b+normal4;\n\tEmitVertex();\n\tEndPrimitive();\n}\n",
		"#version 330 core\n\n\n\nout vec4 out_color;\n\n\n\nvoid main(void){\n\tout_color=vec4(0.7,0.7,0.7,1.0);\n}\n",
		_shader_uniforms_line,
		2
	},
	{
		"#version 330 core\n\n\n\nlayout (location=0) in vec4 in_pos;\nout float gs_red_color;\nuniform mat3 vs_transform;\n\n\n\nvoid main(void){\n\tgl_Position=vec4((vs_transform*vec3(in_pos.xy,1.0)).xy,0.0,1.0);\n\tgs_red_color=(in_pos.w==0?0.05:1.0);\n}\n",
		"#version 330 core\n\n\n\nlayout(points) in;\nlayout(triangle_strip, max_vertices=4) out;\nin float gs_red_color[1];\nout float fs_red_color;\nuniform float gs_half_size;\n\n\n\nvoid main(void){\n\tvec4 pos=gl_in[0].gl_Position;\n\tgl_Position=pos+vec4(-gs_half_size,-gs_half_size,0.0,0.0);\n\tfs_red_color=gs_red_color[0];\n\tEmitVertex();\n\tgl_Position=pos+vec4(gs_half_size,-gs_half_size,0.0,0.0);\n\tfs_red_color=gs_red_color[0];\n\tEmitVertex();\n\tgl_Position=pos+vec4(-gs_half_size,gs_half_size,0.0,0.0);\n\tfs_red_color=gs_red_color[0];\n\tEmitVertex();\n\tgl_Position=pos+vec4(gs_half_size,gs_half_size,0.0,0.0);\n\tfs_red_color=gs_red_color[0];\n\tEmitVertex();\n\tEndPrimitive();\n}\n",
		"#version 330 core\n\n\n\nin float fs_red_color;\nout vec4 out_color;\n\n\n\nvoid main(void){\n\tout_color=vec4(fs_red_color,0.05,0.05,1.0);\n}\n",
		_shader_uniforms_point,
		2
	},
};
