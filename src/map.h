#ifndef MAP_H
#define MAP_H

#include "common.h"
#include "cube.h"

// #define map(x,y,z) (map->map[(u32)((i32)(map->length*map->height)*(x+(i32)map->width/2) + (i32)map->length*(y+(i32)map->length/2) + z+(i32)map->height/2)])
#define map_v(x,y,z) (((abs(x))<=(i32)map->width/2&&(abs(y))<=(i32)map->height/2&&(abs(z))<=(i32)map->length/2)?TRUE:FALSE)
#define offset(x,y,z) _offset(x,y,z,map)

struct map_t {
	struct cube_t *cubes;
	u64 width, length, height;
	u64 size;
	u64 index;
	i32 xmin, ymin, xmax, ymax, zmin, zmax;
	u64 *m;
	u64 m_size;
};


struct map_t * map_create(void);
void map_destroy(struct map_t *map);
void map_empty_cubes(struct map_t *map);
u64 _offset(i32 x, i32 y, i32 z, struct map_t *map);
void map_load(struct map_t *map, f64 time);

#endif
