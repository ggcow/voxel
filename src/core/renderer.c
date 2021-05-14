#define _POSIX_C_SOURCE 200809L


#include <stdio.h>

#include "core/renderer.h"
#include "matrix.h"
#include "core/shader.h"
#include "draw.h"

#define M_PI 3.14192
#define radians(d) ((d) * M_PI / 180.0)

static const GLchar *_2D_VERTEX_SHADER_SOURCE = 

"#version 330 core\n"

"layout(location = 0) in vec3 vertexPosition_modelspace;"
  
"uniform mat4 MVP;"
"out vec3 fragmentColor;"
  
"void main(){"
"	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);"
"	fragmentColor = vertexPosition_modelspace;"
"}"
;


static const GLchar *_2D_FRAGMENT_SHADER_SOURCE = 
"#version 330 core\n"
"out vec3 color;"
"in vec3 fragmentColor;"
"void main(){"
"	color = mod((fragmentColor+gl_PrimitiveID+10),10)/10;"
// "color = vec3(1,0,0);"
"}"
;

void renderer_draw(renderer_t *renderer, map_t *map, player_t *player, u32 width, u32 height) {
	f32 temp[16];

	f32 projection[16];
	_matrix_perspective(radians(90.0f), (f32) width / (f32)height, 0.1f, 1000.0f, projection);

	f32 view[16];
	f32 up[3]={0,1,0};
	f32 center[3];

	_matrix_lookAt(player->eye, player->look, up, view);

	f32 model[16];
	_matrix_identity(model);

	f32 mvp[16];
	_matrix_multiply(model, view, temp);
	_matrix_multiply(projection, temp, mvp);

	GLuint MatrixID = glGetUniformLocation(renderer->program, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0]);

	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, renderer->element_buffer.index, GL_UNSIGNED_INT, (void *)0);
}



static bool _setup(renderer_t *renderer, map_t *map) {
	renderer->program = _create_shader_program(
		_2D_VERTEX_SHADER_SOURCE,
		_2D_FRAGMENT_SHADER_SOURCE
	);

	if (renderer->program == 0) {
		return FALSE;
	}


	buffer_init(renderer->vertex_buffer);
	buffer_init(renderer->element_buffer);

	draw_cubes(renderer, map);

	//map_log_from_above(_map);

	glGenVertexArrays(1, &(renderer->vao));
	glGenBuffers(1, &(renderer->vbo));
	glGenBuffers(1, &(renderer->ebo));

	glBindVertexArray(renderer->vao);

//	log_info("render : %u vertices", renderer->vertex_buffer.index/3);
//	for (u32 i=0; i<renderer->vertex_buffer.index; i+=3) {
//	    log_info("%d %d %d", renderer->vertex_buffer.data[i], renderer->vertex_buffer.data[i+1], renderer->vertex_buffer.data[i+2]);
//	}
//
//    log_info("render : %u triangles", renderer->element_buffer.index/3);
//    for (u32 i=0; i<renderer->element_buffer.index; i+=3) {
//        log_info("%d %d %d", renderer->element_buffer.data[i], renderer->element_buffer.data[i+1], renderer->element_buffer.data[i+2]);
//    }

	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLint)*(renderer->vertex_buffer.index), renderer->vertex_buffer.data, GL_STATIC_DRAW);

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*(renderer->element_buffer.index), renderer->element_buffer.data, GL_STATIC_DRAW);


	renderer_set_clear_color(renderer, 0.0f, 0.0f, 0.0f, 0.0f);
	glClearColor(renderer->clear_color[0], renderer->clear_color[1], renderer->clear_color[2], renderer->clear_color[3]);

	

	glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);  

	glUseProgram(renderer->program);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	return TRUE;
}


renderer_t * renderer_create(map_t *map) {
	renderer_t *renderer = allocate(sizeof(renderer_t));

	if (!_setup(renderer, map)) {
		return NULL;
	}

	log_debug("Renderer created");
	return renderer;
}

void renderer_destroy(renderer_t *renderer) {
	buffer_terminate(renderer->vertex_buffer);
	buffer_terminate(renderer->element_buffer);
	deallocate(renderer);
	log_debug("Renderer destroyed");
}

void renderer_set_clear_color(renderer_t *renderer, f32 r, f32 g, f32 b, f32 a) {
	renderer->clear_color[0] = r;
	renderer->clear_color[1] = g;
	renderer->clear_color[2] = b;
	renderer->clear_color[3] = a;
}



