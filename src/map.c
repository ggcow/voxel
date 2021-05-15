#include "map.h"
#include <math.h>
#include <stdlib.h>


static bool _equation(f32 x, f32 y, f32 z) {
	//f32 e = ((x*x+y*y-0.85*0.85)*(x*x+y*y-0.85*0.85)+(z*z-1)*(z*z-1))*((z*z+y*y-0.85*0.85)*(z*z+y*y-0.85*0.85)+(x*x-1)*(x*x-1))*((x*x+z*z-0.85*0.85)*(x*x+z*z-0.85*0.85)+(y*y-1)*(y*y-1));
	
	f32 e = x*x+y*y+z*z;
	return fabsf(e) <= 1.0f;
}

map_t * map_create(void) {
	map_t *map = allocate(sizeof(map_t));

	buffer_init(map->cube_buffer);
    map->cube_buffer.index = 1;

	map_load(map);

	//error handling ?

//	 log_debug("MAP index : %u", map->cube_buffer.index);
//	 log_debug("xmin : %d; xmax : %d ; ymin : %d ; ymax : %d ; zmin : %d ; zmax : %d",
//		 map->xmin, map->xmax, map->ymin, map->ymax, map->zmin, map->zmax);

    map_log_from_above(map);

	log_debug("Map created");
	return map;
}

void map_destroy(map_t *map) {
    deallocate(map->_map);
	buffer_terminate(map->cube_buffer);
	deallocate(map);
	log_debug("Map destroyed");
}

void map_load(map_t *map) {
	int taille = 100;
	taille |= 1;
	map->width = taille;
	map->length = taille;
	map->height = taille;
	f32 e = 1.0f;

	map->_map = callocate(sizeof(u32), map->width * map->length * map->height);

	i32 width_2 = map->width/2;
	i32 length_2 = map->length/2;
	i32 height_2 = map->height/2;

	buffer_check_size(map->cube_buffer, map->height*map->width*map->length);

	int loading = 0;

	for(i32 i=-width_2; i<=width_2; i++) {

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

		for(i32 j=-length_2; j<=length_2; j++) {
			for(i32 k=-height_2; k<=height_2; k++) {
				if(_equation(e*i/width_2, e*j/length_2, e*k/height_2)) {
                    map_s(j, k, i, map->cube_buffer.index);
				    buffer_push(map->cube_buffer, ((cube_t) {i, k, j}));
				}
			}	
		}
	}

	for (int i=1; i<map->cube_buffer.index; i++) {
		if (map->xmin > map->cube_buffer.data[i].x || i==1)
			map->xmin = map->cube_buffer.data[i].x;
		if (map->xmax < map->cube_buffer.data[i].x || i==1)
			map->xmax = map->cube_buffer.data[i].x;
		if (map->zmin > map->cube_buffer.data[i].z || i==1)
			map->zmin = map->cube_buffer.data[i].z;
		if (map->zmax < map->cube_buffer.data[i].z || i==1)
			map->zmax = map->cube_buffer.data[i].z;
		if (map->ymin > map->cube_buffer.data[i].y || i==1)
			map->ymin = map->cube_buffer.data[i].y;
		if (map->ymax < map->cube_buffer.data[i].y || i==1)
			map->ymax = map->cube_buffer.data[i].y;
	}

	log_command("A");
	log_command("2K");
}

void map_log_from_above(map_t *map) {
	if(map->width*map->length > 49) {
		return;
	}
	printf("\nMAP FROM ABOVE from %d to %d and %d to %d\n\n", map->zmin, map->zmax, map->xmin, map->xmax);
	for (int i=map->xmax; i>=map->xmin; i--) {
		for (int j=map->zmin; j<=map->zmax; j++) {
			int cube=0;
			for (int k=(map->height/2); k>=-(map->height/2); k--)
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

bool map_verify(i32 x, i32 y, i32 z, map_t *map) {
    return -(map->width/2) <= x && x <= map->width/2
           && -(map->length/2) <= y && y <= map->length/2
           && -(map->height/2) <= z && z <= map->height/2;
}

u32 map_get(i32 x, i32 y, i32 z, map_t *map) {
    return map_verify(x, y, z, map) ? map->_map[(u32)(map->length * map->height * (x + map->width/2)
                           + map->length*(y+map->length/2)
                           + z +map->height/2)] : 0;
}

void map_set(i32 x, i32 y, i32 z, u32 index, map_t *map) {
    map->_map[(u32)(map->length * map->height * (x + map->width/2)
                    + map->length*(y+map->length/2)
                    + z + map->height/2)] = index;
}