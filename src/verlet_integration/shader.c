#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <verlet_integration/generated/shaders.h>
#include <verlet_integration/shader.h>



void shader_deinit(void){
	for (unsigned int i=0;i<SHADER_COUNT;i++){
		glDeleteProgram(_compiled_programs[i]);
		_compiled_programs[i]=0;
	}
}



void shader_init(void){
	GLint success;
	GLchar buffer[512];
	const shader_program_source_t* source=_program_sources;
	for (unsigned int i=0;i<SHADER_COUNT;i++){
		_compiled_programs[i]=glCreateProgram();
		unsigned int vertex_shader_id=glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader_id,1,&(source->vertex),NULL);
		glCompileShader(vertex_shader_id);
		glGetShaderiv(vertex_shader_id,GL_COMPILE_STATUS,&success);
		if (!success){
			glGetShaderInfoLog(vertex_shader_id,512,NULL,buffer);
			goto _error;
		}
		glAttachShader(_compiled_programs[i],vertex_shader_id);
		unsigned int geometry_shader_id=glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_shader_id,1,&(source->geometry),NULL);
		glCompileShader(geometry_shader_id);
		glGetShaderiv(geometry_shader_id,GL_COMPILE_STATUS,&success);
		if (!success){
			glGetShaderInfoLog(geometry_shader_id,512,NULL,buffer);
			goto _error;
		}
		glAttachShader(_compiled_programs[i],geometry_shader_id);
		unsigned int fragment_shader_id=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader_id,1,&(source->fragment),NULL);
		glCompileShader(fragment_shader_id);
		glGetShaderiv(fragment_shader_id,GL_COMPILE_STATUS,&success);
		if (!success){
			glGetShaderInfoLog(fragment_shader_id,512,NULL,buffer);
			goto _error;
		}
		glAttachShader(_compiled_programs[i],fragment_shader_id);
		glLinkProgram(_compiled_programs[i]);
		glGetProgramiv(_compiled_programs[i],GL_LINK_STATUS,&success);
		if (!success){
			glGetProgramInfoLog(_compiled_programs[i],512,NULL,buffer);
			goto _error;
		}
		glDeleteShader(vertex_shader_id);
		glDeleteShader(geometry_shader_id);
		glDeleteShader(fragment_shader_id);
		const shader_program_source_uniform_t* uniform=source->uniforms;
		for (unsigned int j=0;j<source->uniform_count;j++){
			_program_uniforms[uniform->offset]=glGetUniformLocation(_compiled_programs[i],uniform->name);
			if (_program_uniforms[uniform->offset]==0xffffffff){
				snprintf(buffer,512,"Unable to find uniform '%s' [%u]\n",uniform->name,uniform->offset);
				goto _error;
			}
			uniform++;
		}
		source++;
	}
	return;
_error:
	puts(buffer);
	_Exit(1);
}
