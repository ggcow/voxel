#ifndef MAP_H
#define MAP_H

#include "common.h"
#include "cube.h"

//#define _map(x,y,z) (_map->_map[(u32)((i32)(_map->length*_map->height)*(x+(i32)_map->width/2) + (i32)_map->length*(y+(i32)_map->length/2) + z+(i32)_map->height/2)])
//#define map_v(x,y,z) ((((x)>0?(x):-(x))<=(i32)_map->width/2&&((y)>0?(y):-(y))<=(i32)_map->length/2&&((z)>0?(z):-(z))<=(i32)_map->height/2)?TRUE:FALSE)
#define map_g(x,y,z) map_get(x,y,z,map)
#define map_v(x,y,z) map_verify(x,y,z,map)
#define map_s(x,y,z,i) map_set(x,y,z,i,map)


struct map_t {
	struct cube_t *cubes;
	i32 width, length, height;
	u32 size;
	u32 index;
	i32 xmin, ymin, xmax, ymax, zmin, zmax;
	u32 *_map;
};


struct map_t * map_create(void);
void map_destroy(struct map_t *map);
void map_load(struct map_t *map);
void map_log_from_above(struct map_t *map);
static void _check_map_size(struct map_t *map, u32 added_size);
u32 map_get(i32 x, i32 y, i32 z, struct map_t *map);
void map_set(i32 x, i32 y, i32 z, u32 index, struct map_t *map);
bool map_verify(i32 x, i32 y, i32 z, struct map_t *map);
#endif
