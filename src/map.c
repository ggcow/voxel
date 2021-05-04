#include "../include/map.h"
#include <math.h>
#include <stdlib.h>


bool
_equation(f32 x, f32 y, f32 z) {
	//f32 e = ((x*x+y*y-0.85*0.85)*(x*x+y*y-0.85*0.85)+(z*z-1)*(z*z-1))*((z*z+y*y-0.85*0.85)*(z*z+y*y-0.85*0.85)+(x*x-1)*(x*x-1))*((x*x+z*z-0.85*0.85)*(x*x+z*z-0.85*0.85)+(y*y-1)*(y*y-1));
	
	f32 e = x*x+y*y+z*z;
	return fabsf(e) < 1.0f;
}

struct map_t *
map_create(void)
{
	struct map_t *map = allocate(sizeof(struct map_t), 1);

	map->size=1024;

	map_load(map);

	if (map->cubes == NULL) {
		log_error("Cannot load map");
		return NULL;
	}


	// log_debug("MAP index : %u", map->index);
	// log_debug("xmin : %d; xmax : %d ; ymin : %d ; ymax : %d ; zmin : %d ; zmax : %d",
		// map->xmin, map->xmax, map->ymin, map->ymax, map->zmin, map->zmax);

	log_debug("Map created");
	return map;
}

void
map_destroy(struct map_t *map)
{
    deallocate(map->map);
	deallocate(map->cubes);
	deallocate(map);
	log_debug("Map destroyed");
}

void
map_load(struct map_t *map)
{
	map->cubes = allocate(sizeof(struct cube_t), map->size);
	map->index = 1;
	int taille = 10;
	taille = (taille*2) | 1;
	map->width = taille+1;
	map->length = taille+1;
	map->height = taille+1;
	f32 e = 1.0f;

	map->map = allocate(sizeof(u32), map->width*map->length*map->height);

	i32 width_2 = (i32)map->width/2;
	i32 length_2 = (i32)map->length/2;
	i32 height_2 = (i32)map->height/2;

	_check_map_size(map, 1+(u32)(map->height*map->width*map->length));

	int loading = 0;

	for(int i=-width_2; i<=width_2; i++) {

		if ((10.0f*(i+width_2)/(int)map->width) > loading) {
			loading++;
			log_command("A");
			log_command("2K");
			info("Loading map : [");
			for (int j=0; j<10; j++) {
				if (j<loading) {
					fprintf(stderr, "#");
				} else {
					fprintf(stderr, " ");
				}
			}
			fprintf(stderr, "]\n");
		}

		for(int j=-length_2; j<=length_2; j++) {
			for(int k=-height_2; k<=height_2; k++) {
				if(_equation(e*i/width_2, e*j/length_2, e*k/height_2)) {
					map->cubes[map->index].x = j;
					map->cubes[map->index].y = k;
					map->cubes[map->index].z = i;
					map_s(j, k, i, map->index);
					map->index++;
				}
			}	
		}
	}

	map->xmin = map->cubes[0].x;
	map->xmax = map->cubes[0].x;
	map->zmin = map->cubes[0].z;
	map->zmax = map->cubes[0].z;
	map->ymin = map->cubes[0].y;
	map->ymax = map->cubes[0].y;

	for (int i=0; i<map->index; i++) {
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
	log_command("A");
	log_command("2K");
	map_log_from_above(map);
}

void
map_log_from_above(struct map_t *map) 
{
	if(map->width*map->length > 30) {
		return;
	}
	printf("\nMAP FROM ABOVE from %d to %d and %d to %d\n\n", map->zmin, map->zmax, map->xmin, map->xmax);

	for (int i=map->xmax; i>=map->xmin; i--) {
		for (int j=map->zmin; j<=map->zmax; j++) {
			int cube=0;
			for (int k=-(int)(map->height/2); k>=-(int)(map->height/2); k--) 
				if (map_g(i,k,j)>0) {
					cube++;
					printf("[]");
					break;
				}
			if (cube==0)
				printf("  ");
		}
		printf("\n");
	} printf("\n"); fflush(stdout);
}

static void
_check_map_size(struct map_t *map, u32 added_size)
{
	while (map->index + added_size >= map->size) {
		map->size *= 2;
		map->cubes = reallocate(map->cubes, sizeof(struct cube_t), map->size);
	}
}

bool
map_verify(i32 x, i32 y, i32 z, struct map_t *map)
{
    return -map->width/2 <= x && x <= map->width/2
           && -map->length/2 <= y && y <= map->length/2
           && -map->height/2 <= z && z <= map->height/2;
}

u32
map_get(i32 x, i32 y, i32 z, struct map_t *map)
{
    return map->map[(u32)((i32)(map->length*map->height)*(x+(i32)map->width/2)
                          + (i32)map->length*(y+(i32)map->length/2)
                          + z+(i32)map->height/2)];
}

void
map_set(i32 x, i32 y, i32 z, u32 index, struct map_t *map)
{
    map->map[(u32)((i32)(map->length*map->height)*(x+(i32)map->width/2)
                          + (i32)map->length*(y+(i32)map->length/2)
                          + z+(i32)map->height/2)] = index;
}