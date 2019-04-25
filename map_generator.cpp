#include "map_generator.hpp"
#include <random>

#define CORNER_WIDTH 3
#define CORNER_HEIGHT 3

static OBJECT map_stage_one[CORNER_WIDTH * CORNER_HEIGHT] = {
	  _,   _,   D,
	  _,   W,   _,
	  D,   _,   _,
};

static int map_stage_one_mask[CORNER_WIDTH * CORNER_HEIGHT] = {
	  1,   1,   1,
	  1,   0,   0,
	  1,   0,   0,
};

void generate_map(OBJECT* dest, int map_width, int map_height)
{
  // Make space
  OBJECT* _map = new OBJECT[map_width * map_height];

  // Generate random debris
  for (int i = 0, length = map_width * map_height; i < length; ++i)
  {
    bool put_debris = (rand() % 100) < 50;
    OBJECT t = put_debris ? D : _;

    _map[i] = t;
  }

  // Add walls
  for (int y = 0; y < map_width; ++y)
  {
    if (y % 2 != 0)
    {
      for (int x = 0; x < map_height; ++x)
      {
        if (x % 2 != 0)
        {
          _map[y * map_width + x] = W;
        }
      }
    }
  }

  // Leave a corner for dyna
  for (int y = 0; y < CORNER_HEIGHT; ++y)
  {
    for (int x = 0; x < CORNER_WIDTH; ++x)
    {
      if (map_stage_one_mask[y * CORNER_WIDTH + x] == 1)
      {
        _map[y * map_width + x] = map_stage_one[y * CORNER_WIDTH + x];
      }
    }
  }

  set_up_map(_map, dest, map_width, map_height);

  delete[] _map;
}
