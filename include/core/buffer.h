#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"
#include "opengl.h"
#include "cube.h"

#define buffer_type(type) struct buffer_##type##_t

#define define_buffer_type(type) buffer_type(type)  {                                           \
    u32 index;                                                                                  \
    u32 size;                                                                                   \
    type *data;                                                                                 \
}

define_buffer_type(GLuint);
define_buffer_type(GLint);
define_buffer_type(cube_t);

#undef define_buffer_type

#define buffer_init(buffer)                                                                     \
buffer.index = 0;                                                                               \
buffer.size = 32;                                                                               \
buffer.data = allocate(sizeof(*(((typeof(buffer)*)0)->data))*buffer.size);

#define buffer_check_size(buffer, added_size)                                                   \
while (buffer.index + added_size >= buffer.size) {                                              \
    buffer.size *= 2;                                                                           \
    buffer.data = reallocate(buffer.data, sizeof(*buffer.data) * buffer.size);                  \
}

#define buffer_push(buffer, value) buffer.data[buffer.index++] = value
#define buffer_terminate(buffer) deallocate(buffer.data);


#endif
