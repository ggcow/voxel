#include "common.h"
#include "log.h"

#include <stdlib.h>

void * _allocate(const char *filename, u32 line, usize size) {
	void *pointer = malloc(size);

	if (pointer == NULL) {
		log_error("Allocation failed in %s:%d", filename, line);
	}

	return pointer;
}

void * _callocate(const char *filename, u32 line, usize size) {
    void *pointer = calloc(1, size);

    if (pointer == NULL) {
        log_error("Allocation failed in %s:%d", filename, line);
    }

    return pointer;
}

void * _reallocate(const char *filename, u32 line, void *pointer, usize size) {
	if (pointer == NULL) {
		log_warning("Trying to reallocate NULL pointer in %s:%d", filename, line);

		return allocate(size);
	}

	return realloc(pointer, size);
}

void _deallocate(const char *filename, u32 line, void *pointer) {
	if (pointer == NULL) {
		log_warning("Trying to deallocate NULL pointer in %s:%d", filename, line);
	} else {
		free(pointer);
	}
}