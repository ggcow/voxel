#ifndef MAP_H
#define MAP_H

#include "common.h"
#include "cube.h"

#define map(x,y,z) (map->map[(u32)((i32)(map->length*map->height)*(x+(i32)map->width/2) + (i32)map->length*(y+(i32)map->length/2) + z+(i32)map->height/2)])
#define map_v(x,y,z) ((((x)>0?(x):-(x))<=(i32)map->width/2&&((y)>0?(y):-(y))<=(i32)map->length/2&&((z)>0?(z):-(z))<=(i32)map->height/2)?TRUE:FALSE)

struct map_t {
	struct cube_t *cubes;
	u32 width, length, height;
	u32 size;
	u32 index;
	i32 xmin, ymin, xmax, ymax, zmin, zmax;
	u32 *map;
};


struct map_t * map_create(void);
void map_destroy(struct map_t *map);
void map_load(struct map_t *map);
void map_log_from_above(struct map_t *map);
static void _check_map_size(struct map_t *map, u32 added_size);

#endif
