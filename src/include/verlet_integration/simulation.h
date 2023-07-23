#ifndef _VERLET_INTEGRATION_SIMULATION_H_
#define _VERLET_INTEGRATION_SIMULATION_H_ 1



typedef struct _SIMULATION_POINT{
	float x;
	float y;
	float z;
	unsigned int mask;
} simulation_point_t;



typedef struct _SIMULATION_CONNECTION_POINTS{
	unsigned int a;
	unsigned int b;
} simulation_connection_points_t;



typedef struct _SIMULATION_RENDERER_CONTEXT{
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int vbo_size;
	unsigned int ebo_size;
} simulation_renderer_context_t;



typedef struct _SIMULATION_CONTEXT{
	unsigned int point_count;
	unsigned int connection_count;
	simulation_point_t* points;
	simulation_connection_points_t* connection_points;
	simulation_point_t* _prev_points;
	float* _connection_quarter_lengths;
} simulation_context_t;



void simulation_deinit(simulation_context_t* ctx);



void simulation_renderer_deinit(simulation_renderer_context_t* ctx);



void simulation_init(simulation_context_t* ctx);



void simulation_renderer_init(simulation_renderer_context_t* ctx);



void simulation_update(simulation_context_t* ctx,float delta_time);



void simulation_draw(simulation_context_t* ctx,simulation_renderer_context_t* renderer_ctx);



unsigned int simulation_add_point(simulation_context_t* ctx,float x,float y,float z,_Bool fixed);



unsigned int simulation_add_connection(simulation_context_t* ctx,unsigned int a,unsigned int b,float length);



#endif
