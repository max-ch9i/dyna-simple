#include "Utils.hpp"

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

OBJECT tile_at(const OBJECT* map_mat, int map_width, int map_height, XY coord)
{
  if (coord.x >= map_width || coord.y >= map_height)
  {
    return B;
  }
  else if (coord.x < 0 || coord.y < 0)
  {
    return B;
  }

  return map_mat[coord.y * map_width + coord.x];
}

XY pos_by_dir(const XY& pos, DIRE dir,int speed = 1)
{
  XY next_pos(pos);

  switch (dir)
  {
    case LEFT:
      next_pos.x -= speed;
      break;
    case RIGHT:
      next_pos.x += speed;
      break;
    case DOWN:
      next_pos.y += speed;
      break;
    case UP:
      next_pos.y -= speed;
      break;
  }

  return next_pos;
}

void swap_tile_to(OBJECT* map_mat, int map_width, const XY& p, OBJECT swap_to)
{
  map_mat[p.y * map_width + p.x] = swap_to;
}

void incinerate_wall_at(const XY& p)
{
  swap_tile_to(map, map_width, p, _);
}

void set_up_map(const OBJECT* src, OBJECT* dest, int map_width, int map_height)
{
  for (int i = 0, length = map_width * map_height; i < length; ++i)
  {
    dest[i] = src[i];
  }
}
