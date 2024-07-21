#include <GL/gl.h>
#include <verlet_integration/engine.h>
#include <verlet_integration/simulation.h>



static simulation_context_t _simulation_data;
static simulation_renderer_context_t _simulation_renderer_data;



void renderer_deinit(void){
	simulation_deinit(&_simulation_data);
	simulation_renderer_deinit(&_simulation_renderer_data);
}



void renderer_init(void){
	simulation_init(&_simulation_data);
	simulation_renderer_init(&_simulation_renderer_data);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	unsigned int tmp=simulation_add_point(&_simulation_data,-1024.0f,-1024.0f,0.0f,1);
	unsigned int a=simulation_add_point(&_simulation_data,512.0f,128.0f,0.0f,1);
	unsigned int b=simulation_add_point(&_simulation_data,768.0f,128.0f,0.0f,0);
	unsigned int c=simulation_add_point(&_simulation_data,1024.0f,128.0f,0.0f,0);
	simulation_add_connection(&_simulation_data,a,b,256.0f);
	simulation_add_connection(&_simulation_data,tmp,tmp,1.0f); // consecutive connections cannot share points
	simulation_add_connection(&_simulation_data,b,c,256.0f);
}



void renderer_tick(float delta_time){
	simulation_update(&_simulation_data,delta_time);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	simulation_draw(&_simulation_data,&_simulation_renderer_data);
}
