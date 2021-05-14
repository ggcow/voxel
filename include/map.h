#ifndef MAP_H
#define MAP_H

#include "core/common.h"
#include "cube.h"
#include "core/buffer.h"

//#define _map(x,y,z) (_map->_map[(u32)((i32)(_map->length*_map->height)*(x+(i32)_map->width/2) + (i32)_map->length*(y+(i32)_map->length/2) + z+(i32)_map->height/2)])
//#define map_v(x,y,z) ((((x)>0?(x):-(x))<=(i32)_map->width/2&&((y)>0?(y):-(y))<=(i32)_map->length/2&&((z)>0?(z):-(z))<=(i32)_map->height/2)?TRUE:FALSE)
#define map_g(x,y,z) map_get(x,y,z,map)
#define map_v(x,y,z) map_verify(x,y,z,map)
#define map_s(x,y,z,i) map_set(x,y,z,i,map)


typedef struct {
	buffer_type(cube_t) cube_buffer;
	i32 width, length, height;
	i32 xmin, ymin, xmax, ymax, zmin, zmax;
	u32 *_map;
} map_t;


map_t * map_create(void);
void map_destroy(map_t *map);
void map_load(map_t *map);
void map_log_from_above(map_t *map);

u32 map_get(i32 x, i32 y, i32 z, map_t *map);
void map_set(i32 x, i32 y, i32 z, u32 index, map_t *map);
bool map_verify(i32 x, i32 y, i32 z, map_t *map);
#endif
