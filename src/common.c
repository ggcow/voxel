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

void * _callocate(const char *filename, u32 line, usize size, usize count) {
    void *pointer = calloc(size, count);

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

u32 string_compare(const char *left, const char *right) {
	usize i = 0;

	while (left[i] != 0 && right[i] != 0 && left[i] == right[i]) {
		i++;
	}

	return left[i] < right[i] ? -1 : left[i] > right[i] ? 1 : 0;
}
