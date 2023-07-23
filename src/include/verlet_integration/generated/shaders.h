#ifndef _VERLET_INTEGRATION_GENERATED_SHADERS_H_
#define _VERLET_INTEGRATION_GENERATED_SHADERS_H_ 1



#define SHADER_COUNT 2

#define SHADER_LINE ((shader_id_t)_compiled_programs[0])
#define SHADER_LINE_vs_transform (_program_uniforms[0])
#define SHADER_LINE_gs_half_size (_program_uniforms[1])
#define SHADER_POINT ((shader_id_t)_compiled_programs[1])
#define SHADER_POINT_vs_transform (_program_uniforms[2])
#define SHADER_POINT_gs_half_size (_program_uniforms[3])



typedef struct _SHADER_PROGRAM_SOURCE_UNIFORM{
	const char* name;
	unsigned int offset;
} shader_program_source_uniform_t;



typedef struct _SHADER_PROGRAM_SOURCE{
	const char* vertex;
	const char* geometry;
	const char* fragment;
	const shader_program_source_uniform_t* uniforms;
	unsigned int uniform_count;
} shader_program_source_t;



extern unsigned int _compiled_programs[2];
extern unsigned int _program_uniforms[4];
extern const shader_program_source_t _program_sources[2];



#endif
