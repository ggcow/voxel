#include "buffer.h"

pbuffer_t pbuffer_make(void (*data_destroy)(void *))
{
    pbuffer_t buffer = (pbuffer_t) {data_destroy, 0, 32};
    buffer.data = allocate(sizeof(void *) * buffer.size);
    return buffer;
}

void pbuffer_terminate(pbuffer_t buffer)
{
    for (int i=0; i<buffer.index; i++) {
        if (buffer.data[i]) {
            buffer.data_destroy(buffer.data[i]);
        }
    }
    deallocate(buffer.data);
}

int iu_buffer_loop_check(int b)
{
    static int s_b = 1;
    int r_v = s_b;
    s_b = b;
    return r_v;
}