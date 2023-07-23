#include <GL/gl.h>
#include <immintrin.h>
#include <stdlib.h>
#include <string.h>
#include <verlet_integration/engine.h>
#include <verlet_integration/shader.h>
#include <verlet_integration/simulation.h>



#define POINT_SIZE 20.0f
#define LINE_SIZE 8.0f

#define CONNECTION_UPDATE_COUNT 200

#define DRAG 0.9985f
#define GRAVITY 9.81f



void simulation_deinit(simulation_context_t* ctx){
	free(ctx->points);
	free(ctx->connection_points);
	free(ctx->_prev_points);
	free(ctx->_connection_quarter_lengths);
}



void simulation_renderer_deinit(simulation_renderer_context_t* ctx){
	glDeleteBuffers(1,&(ctx->ebo));
	glDeleteBuffers(1,&(ctx->vbo));
	glDeleteVertexArrays(1,&(ctx->vao));
}



void simulation_init(simulation_context_t* ctx){
	ctx->point_count=0;
	ctx->connection_count=0;
	ctx->points=NULL;
	ctx->connection_points=NULL;
	ctx->_prev_points=NULL;
	ctx->_connection_quarter_lengths=NULL;
	simulation_add_point(ctx,0.0f,0.0f,0.0f,1);
}



void simulation_renderer_init(simulation_renderer_context_t* ctx){
	glGenVertexArrays(1,&(ctx->vao));
	glGenBuffers(1,&(ctx->vbo));
	glGenBuffers(1,&(ctx->ebo));
	glBindVertexArray(ctx->vao);
	glBindBuffer(GL_ARRAY_BUFFER,ctx->vbo);
	glBufferData(GL_ARRAY_BUFFER,0,NULL,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ctx->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,0,NULL,GL_DYNAMIC_DRAW);
	ctx->vbo_size=0;
	ctx->ebo_size=0;
}



void simulation_update(simulation_context_t* ctx,float delta_time){
	__m256 const_drag=_mm256_set_ps(0,DRAG,DRAG,DRAG,0,DRAG,DRAG,DRAG);
	__m256 const_gravity=_mm256_set_ps(0,0,GRAVITY*delta_time,0,0,0,GRAVITY*delta_time,0);
	float* point_coords=(float*)(ctx->points);
	float* prev_point_coords=(float*)(ctx->_prev_points);
	for (unsigned int i=0;i<ctx->point_count;i+=2){
		__m256 xyzw=_mm256_load_ps(point_coords);
		_mm256_store_ps(
			point_coords,
			_mm256_add_ps(
				xyzw,
				_mm256_andnot_ps(
					_mm256_permute_ps(
						xyzw,
						0b11111111
					),
					_mm256_fmadd_ps(
						_mm256_sub_ps(
							xyzw,
							_mm256_load_ps(
								prev_point_coords
							)
						),
						const_drag,
						const_gravity
					)
				)
			)
		);
		_mm256_store_ps(prev_point_coords,xyzw);
		point_coords+=8;
		prev_point_coords+=8;
	}
	__m256i const_zero=_mm256_setzero_si256();
	__m256 const_half=_mm256_set1_ps(0.5f);
	__m256 const_one_x=_mm256_set_ps(0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f);
	__m256 const_three=_mm256_set1_ps(3.0f);
	__m256 const_xyz_mask=_mm256_castsi256_ps(_mm256_set_epi32(0x00000000,0xffffffff,0xffffffff,0xffffffff,0x00000000,0xffffffff,0xffffffff,0xffffffff));
	for (unsigned int i=0;i<CONNECTION_UPDATE_COUNT;i++){
		const simulation_connection_points_t* connection_points=ctx->connection_points;
		const float* connection_quarter_lengths=ctx->_connection_quarter_lengths;
		for (unsigned int j=0;j<ctx->connection_count;j+=2){
			float* ptr_a0=(void*)(((unsigned long long int)(ctx->points))+(connection_points->a<<4));
			float* ptr_a1=(void*)(((unsigned long long int)(ctx->points))+((connection_points+1)->a<<4));
			float* ptr_b0=(void*)(((unsigned long long int)(ctx->points))+(connection_points->b<<4));
			float* ptr_b1=(void*)(((unsigned long long int)(ctx->points))+((connection_points+1)->b<<4));
			__m256 xyzw_a=_mm256_insertf128_ps(
				_mm256_castps128_ps256(
					_mm_load_ps(
						ptr_a0
					)
				),
				_mm_load_ps(
					ptr_a1
				),
				1
			);
			__m256 xyzw_b=_mm256_insertf128_ps(
				_mm256_castps128_ps256(
					_mm_load_ps(
						ptr_b0
					)
				),
				_mm_load_ps(
					ptr_b1
				),
				1
			);
			__m256 d_xyzw_v=_mm256_and_ps(
				_mm256_sub_ps(
					xyzw_b,
					xyzw_a
				),
				const_xyz_mask
			);
			__m256 d_xyzw_sq=_mm256_dp_ps(
				d_xyzw_v,
				d_xyzw_v,
				0b11111111
			);
			__m256 d_xyzw=_mm256_rsqrt_ps(
				d_xyzw_sq
			);
			d_xyzw_v=_mm256_blendv_ps(
				_mm256_mul_ps(
					d_xyzw_v,
					_mm256_permute_ps(
						_mm256_fnmadd_ps(
							_mm256_castpd_ps(
								_mm256_permute_pd(
									_mm256_castps_pd(
										_mm256_permute_ps(
											_mm256_castpd_ps(
												_mm256_broadcast_sd(
													(const double*)connection_quarter_lengths
												)
											),
											0b00000101
										)
									),
									0b0011
								)
							),
							_mm256_mul_ps(
								_mm256_fnmadd_ps(
									_mm256_mul_ps(
										d_xyzw_sq,
										d_xyzw
									),
									d_xyzw,
									const_three
								),
								d_xyzw
							),
							const_half
						),
						0b00000000
					)
				),
				const_one_x,
				_mm256_castsi256_ps(
					_mm256_cmpeq_epi32(
						_mm256_castps_si256(
							d_xyzw_sq
						),
						const_zero
					)
				)
			);
			__m256 mask_a=_mm256_permute_ps(
				xyzw_a,
				0b11111111
			);
			__m256 mask_b=_mm256_permute_ps(
				xyzw_b,
				0b11111111
			);
			d_xyzw_v=_mm256_add_ps(
				d_xyzw_v,
				_mm256_and_ps(
					d_xyzw_v,
					_mm256_or_ps(
						mask_a,
						mask_b
					)
				)
			);
			xyzw_a=_mm256_add_ps(
				xyzw_a,
				_mm256_andnot_ps(
					mask_a,
					d_xyzw_v
				)
			);
			xyzw_b=_mm256_sub_ps(
				xyzw_b,
				_mm256_andnot_ps(
					mask_b,
					d_xyzw_v
				)
			);
			_mm_store_ps(
				ptr_a0,
				_mm256_castps256_ps128(
					xyzw_a
				)
			);
			_mm_store_ps(
				ptr_a1,
				_mm256_extractf128_ps(
					xyzw_a,
					1
				)
			);
			_mm_store_ps(
				ptr_b0,
				_mm256_castps256_ps128(
					xyzw_b
				)
			);
			_mm_store_ps(
				ptr_b1,
				_mm256_extractf128_ps(
					xyzw_b,
					1
				)
			);
			connection_quarter_lengths+=2;
			connection_points+=2;
		}
	}
}



void simulation_draw(simulation_context_t* ctx,simulation_renderer_context_t* renderer_ctx){
	glBindVertexArray(renderer_ctx->vao);
	glBindBuffer(GL_ARRAY_BUFFER,renderer_ctx->vbo);
	unsigned int size=ctx->point_count*sizeof(simulation_point_t);
	if (size!=renderer_ctx->vbo_size){
		renderer_ctx->vbo_size=size;
		glBufferData(GL_ARRAY_BUFFER,size,ctx->points,GL_DYNAMIC_DRAW);
	}
	else{
		glBufferSubData(GL_ARRAY_BUFFER,0,size,ctx->points);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,renderer_ctx->ebo);
	size=ctx->connection_count*sizeof(simulation_connection_points_t);
	if (size!=renderer_ctx->ebo_size){
		renderer_ctx->ebo_size=size;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,ctx->connection_points,GL_DYNAMIC_DRAW);
	}
	else{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,size,ctx->connection_points);
	}
	float transform_matrix[9]={
		2.0f/engine_width,
		0.0f,
		0.0f,
		0.0f,
		-2.0f/engine_height,
		0.0f,
		-1.0f,
		1.0f,
		1.0f,
	};
	glUseProgram(SHADER_POINT);
	glUniformMatrix3fv(SHADER_POINT_vs_transform,1,GL_FALSE,transform_matrix);
	glUniform1f(SHADER_POINT_gs_half_size,transform_matrix[0]*POINT_SIZE);
	glDrawArrays(GL_POINTS,1,ctx->point_count-1);
	glUseProgram(SHADER_LINE);
	glUniformMatrix3fv(SHADER_LINE_vs_transform,1,GL_FALSE,transform_matrix);
	glUniform1f(SHADER_LINE_gs_half_size,transform_matrix[0]*LINE_SIZE);
	glDrawElements(GL_LINES,ctx->connection_count<<1,GL_UNSIGNED_INT,NULL);
}



unsigned int simulation_add_point(simulation_context_t* ctx,float x,float y,float z,_Bool fixed){
	ctx->point_count++;
	simulation_point_t* new_points=aligned_alloc(32,((ctx->point_count+1)&0xfffffffe)*sizeof(simulation_point_t));
	simulation_point_t* new_prev_points=aligned_alloc(32,((ctx->point_count+1)&0xfffffffe)*sizeof(simulation_point_t));
	memcpy(new_points,ctx->points,(ctx->point_count&0xfffffffe)*sizeof(simulation_point_t));
	memcpy(new_prev_points,ctx->_prev_points,(ctx->point_count&0xfffffffe)*sizeof(simulation_point_t));
	(new_points+ctx->point_count-1)->x=x;
	(new_points+ctx->point_count-1)->y=y;
	(new_points+ctx->point_count-1)->z=z;
	(new_points+ctx->point_count-1)->mask=(fixed?0xffffffff:0);
	*(new_prev_points+ctx->point_count-1)=*(new_points+ctx->point_count-1);
	free(ctx->points);
	free(ctx->_prev_points);
	ctx->points=new_points;
	ctx->_prev_points=new_prev_points;
	return ctx->point_count-1;
}



unsigned int simulation_add_connection(simulation_context_t* ctx,unsigned int a,unsigned int b,float length){
	ctx->connection_count++;
	ctx->connection_points=realloc(ctx->connection_points,((ctx->connection_count+1)&0xfffffffe)*sizeof(simulation_connection_points_t));
	ctx->_connection_quarter_lengths=realloc(ctx->_connection_quarter_lengths,((ctx->connection_count+1)&0xfffffffe)*sizeof(float));
	(ctx->connection_points+ctx->connection_count-1)->a=a;
	(ctx->connection_points+ctx->connection_count-1)->b=b;
	*(ctx->_connection_quarter_lengths+ctx->connection_count-1)=length/4.0f;
	if (ctx->connection_count&1){
		(ctx->connection_points+ctx->connection_count)->a=0;
		(ctx->connection_points+ctx->connection_count)->b=0;
		*(ctx->_connection_quarter_lengths+ctx->connection_count)=1.0f;
	}
	return ctx->connection_count-1;
}
