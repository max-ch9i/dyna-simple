#ifndef MAP_GENERATOR_H

#define MAP_GENERATOR_H
#include "Utils.hpp"

void generate_map(OBJECT* dest, int map_width, int map_height);
XY choose_balloon_coord(const OBJECT* map, int map_width, int map_height);

#endif /* end of include guard: MAP_GENERATOR_H */
