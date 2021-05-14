#ifndef COMMON_H
#define COMMON_H

#ifndef __STDC_IEC_559__
#error "Requires IEEE 754 floating point!"
#endif

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>

typedef uint8_t u8;
typedef int8_t i8;

typedef uint16_t u16;
typedef int16_t i16;

typedef uint32_t u32;
typedef int32_t i32;

typedef uint64_t u64;
typedef int64_t i64;

typedef size_t usize;
typedef ssize_t isize;

typedef float f32;
typedef double f64;

typedef enum {
	FALSE = 0,
	TRUE = 1,
} bool;


enum state {
	DOWN = 0,
	UP = 1,
};

#ifndef NULL
#define NULL 0
#endif

void * _allocate(const char *filename, u32 line, usize size);
void *_callocate(const char *filename, u32 line, usize size, usize count);
void * _reallocate(const char *filename, u32 line, void *pointer, usize size);
void _deallocate(const char *filename, u32 line, void *pointer);



#define allocate(...) \
	_allocate(__FILE__, __LINE__, __VA_ARGS__)
#define callocate(...) \
	_callocate(__FILE__, __LINE__, __VA_ARGS__)
#define reallocate(...) \
	_reallocate(__FILE__, __LINE__, __VA_ARGS__)
#define deallocate(...) \
	_deallocate(__FILE__, __LINE__, __VA_ARGS__)

u32 string_compare(const char *left, const char *right);
#include "log.h"
#endif
