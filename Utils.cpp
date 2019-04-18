#include "Utils.hpp"

const int map_width = 5;
const int map_height = 5;
OBJECT _map[map_width * map_height] = {
	  _,   _,   _,   _,   D,
	  _,   W,   _,   W,   _,
	  _,   D,   _,   _,   _,
	  _,   W,   D,   W,   _,
	  _,   _,   _,   _,   _,
};
OBJECT* map = _map;

void win_choose_colour(cairo_t* cr, COLOUR c)
{
  switch(c)
  {
    case COLOUR::GREEN:
      cairo_set_source_rgb(cr, .12, .84, 0);
      break;
    case COLOUR::RED:
      cairo_set_source_rgb(cr, .94, .18, .23);
      break;
    case COLOUR::GREY:
      cairo_set_source_rgb(cr, .85, .85, .85);
      break;
    case COLOUR::BLUE:
      cairo_set_source_rgb(cr, .29, .44, 1);
      break;
    case COLOUR::BROWN:
      cairo_set_source_rgb(cr, .97, .65, .09);
      break;
    case COLOUR::WHITE:
      cairo_set_source_rgb(cr, 1, 1, 1);
      break;
    case COLOUR::BLACK:
      cairo_set_source_rgb(cr, 0, 0, 0);
      break;
    case COLOUR::ORANGE:
      cairo_set_source_rgb(cr, 1, .32, 0);
      break;
  }
}

OBJECT tile_at(const OBJECT* map_mat, int map_width, XY coord)
{
  return map_mat[coord.y * map_width + coord.x];
}

void swap_tile_to(OBJECT* map_mat, int map_width, const XY& p, OBJECT swap_to)
{
  map_mat[p.y * map_width + p.x] = swap_to;
}

void incinerate_wall_at(const XY& p)
{
  swap_tile_to(map, map_width, p, _);
}
