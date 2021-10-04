#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"
#include "opengl.h"

#define buffer_type(type) struct buffer_##type

#define define_buffer_type(type) buffer_type(type)  {                                           \
    i32 index;                                                                                  \
    i32 size;                                                                                   \
    type *data;                                                                                 \
}

define_buffer_type(GLint);

#define buffer_init(buffer)                                                                     \
buffer.index = 0;                                                                               \
(buffer).size = 32;                                                                               \
(buffer).data = allocate(sizeof(*(((typeof(buffer)*)0)->data))*(buffer).size);

#define buffer_check_size(buffer, added_size)                                                   \
while ((buffer).index + (added_size) >= (buffer).size) {                                              \
    (buffer).size *= 2;                                                                           \
    (buffer).data = reallocate((buffer).data, sizeof(*(buffer).data) * (buffer).size);                  \
}

#define buffer_push(buffer, value) buffer.data[(buffer).index++] = value;
#define buffer_terminate(buffer)                                                                \
if ((buffer).data) deallocate((buffer).data);                                                       \
(buffer).data = NULL;

typedef struct pbuffer_t {
    void (*data_destroy)(void *);
    i32 index;
    i32 size;
    void **data;
} pbuffer_t;

pbuffer_t pbuffer_make(void (*data_destroy)(void *));
void pbuffer_terminate(pbuffer_t);
int iu_buffer_loop_check(int b);


#endif
