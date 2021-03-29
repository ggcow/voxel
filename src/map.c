#include "map.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define PROGRESS_BAR 0

static i32 _equation(i32 x, i32 y, f64 t);
static void _check_map_size(struct map_t *map, usize added_size);

static i32
_equation(i32 x, i32 y, f64 t)
{
	f64 h = sin(3.14/2)*20*cos((sqrt(sqrt(x*x+y*y))-t*2));
	return (i32)h;
}


struct map_t *
map_create(void)
{
	struct map_t *map = allocate(sizeof(struct map_t), 1);

	map->size=1024;

	int taille = 100;

	taille=taille*2+1;
	map->width = taille;
	map->length = taille;
	map->height = 41;

	map->m_size = map->width;
	map->m_size*=map->length;
	map->m_size*=map->height;

	map->cubes = allocate(sizeof(struct cube_t), map->size);
	map->m = allocate(sizeof(u64), map->m_size);

	map_load(map, 0);

	// log_debug("MAP index : %u", map->index);
	// log_debug("xmin : %d; xmax : %d ; ymin : %d ; ymax : %d ; zmin : %d ; zmax : %d",
	// 	map->xmin, map->xmax, map->ymin, map->ymax, map->zmin, map->zmax);

	log_debug("Map created");
	return map;
}

void
map_destroy(struct map_t *map)
{
	deallocate(map->cubes);
	deallocate(map->m);
	deallocate(map);
	log_debug("Map destroyed");
}

void
map_empty_cubes(struct map_t *map)
{
	map->index=1;
}

u64
_offset(i32 x, i32 y, i32 z, struct map_t *map)
{ 
	u64 result = 1;
	x+=(i32)map->width/2;
	y+=(i32)map->height/2;
	z+=(i32)map->length/2;

	result*=z;
	result*=map->height;
	result*=map->width;

	result+=y*map->width;

	result+=x;
    return result;
}

void
map_load(struct map_t *map, f64 time)
{
	i32 fill =10;
	
	map_empty_cubes(map);
	
	
	i32 width_2 = (i32)map->width/2;
	i32 length_2 = (i32)map->length/2;
	i32 height_2 = (i32)map->height/2;

	if (PROGRESS_BAR) {
		info(log_real()?"Loading map : [          ]":"Loading map : [");
    	log_command("11D");
    }

	u64 total_size = map->width;
	u64 step_size= total_size/10;
	u64 local_count_max = step_size;
	u64 count = 0;

    u64 reported_count = 0;
    u64 local_count = 0;

	for(int i=-width_2; i<=width_2; i++) {

		if (local_count++ >= local_count_max-1 && PROGRESS_BAR) {
			count += local_count_max;
      		local_count = 0;
		}

		if (count - reported_count >= step_size && PROGRESS_BAR) {
			fprintf(stderr, "#");
			reported_count = count;
		}

		for(int j=-length_2; j<=length_2; j++) {
			i32 y = _equation(i,j,time);
			y=abs(y)<=height_2?y:0; 

			_check_map_size(map, (usize)(map->height));

			for (int k=-height_2; k<=height_2; k++) {
				
				if (((k<=y && k+fill>=y) || (k<=y && fill==-1)) && ((i != 0 && j != 0) || k == y)) {
					map->cubes[map->index].x = j;
					map->cubes[map->index].y = k;
					map->cubes[map->index].z = i;
					map->m[offset(j, k, i)] = map->index; 
					map->index++;
				} else {
					map->m[offset(j, k, i)] = 0;
				}
			}
		}
	}
    
	// map->size = map->index;
	// reallocate(map->cubes, sizeof(struct cube_t), map->size);

	map->xmin = map->cubes[0].x;
	map->xmax = map->cubes[0].x;
	map->zmin = map->cubes[0].z;
	map->zmax = map->cubes[0].z;
	map->ymin = map->cubes[0].y;
	map->ymax = map->cubes[0].y;

	for (u64 i=0; i<map->index; i++) {
		if (map->xmin > map->cubes[i].x)
			map->xmin = map->cubes[i].x;
		if (map->xmax < map->cubes[i].x)
			map->xmax = map->cubes[i].x;
		if (map->zmin > map->cubes[i].z)
			map->zmin = map->cubes[i].z;
		if (map->zmax < map->cubes[i].z)
			map->zmax = map->cubes[i].z;
		if (map->ymin > map->cubes[i].y)
			map->ymin = map->cubes[i].y;
		if (map->ymax < map->cubes[i].y)
			map->ymax = map->cubes[i].y;
	}

	if (PROGRESS_BAR) {
		log_command("2K");
		log_command("40D");
		if (!log_real()) {
			fprintf(stderr, "]\n");
		}
	}

	if (map->cubes == NULL) {
		log_error("Cannot load map");
		exit(1);
	}
}

static void
_check_map_size(struct map_t *map, usize added_size)
{
	while (map->index + added_size >= map->size) {
		map->size *= 2;
		map->cubes = reallocate(map->cubes, sizeof(struct cube_t), map->size);
	}
}