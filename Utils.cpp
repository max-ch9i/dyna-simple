#include "Utils.hpp"

void win_choose_colour(cairo_t* cr, COLOUR c)
{
  switch(c)
  {
    case COLOUR::GREEN:
      cairo_set_source_rgb(cr, 0, 1, 0);
      break;
    case COLOUR::RED:
      cairo_set_source_rgb(cr, 1, 0, 0);
      break;
    case COLOUR::GREY:
      cairo_set_source_rgb(cr, .2, .2, .2);
      break;
    case COLOUR::BLUE:
      cairo_set_source_rgb(cr, 0, 0, 1);
      break;
    case COLOUR::BROWN:
      cairo_set_source_rgb(cr, 1, 0, 1);
      break;
    case COLOUR::WHITE:
      cairo_set_source_rgb(cr, 1, 1, 1);
      break;
    case COLOUR::ORANGE:
      cairo_set_source_rgb(cr, .5, .5, 1);
      break;
  }
}

OBJECT tile_at(const OBJECT* map_mat, int map_width, XY coord)
{
  return map_mat[coord.y * map_width + coord.x];
}
