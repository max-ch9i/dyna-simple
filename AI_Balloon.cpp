#include <array>
#include <random>
#include "Utils.hpp"
#include "AI_Balloon.hpp"

void AI_Balloon::assign(Balloon* _b)
{
  balloon = _b;
}

void AI_Balloon::move()
{
  XY next_pos = pos_by_dir(balloon->pos, dir, balloon->speed);
  OBJECT next_tile = tile_at(map, map_width, map_height, next_pos);

  if (balloon->is_valid_tile(next_tile) && check_if_only_direction())
  {
    balloon->move(dir);
  }
  else
  {
    choose_new_dir();
    balloon->move(dir);
  }
}

bool AI_Balloon::check_if_only_direction()
{
  XY next_pos_1, next_pos_2;
  switch(dir)
  {
    case LEFT:
    case RIGHT:
      next_pos_1 = pos_by_dir(balloon->pos, UP, balloon->speed);
      next_pos_2 = pos_by_dir(balloon->pos, DOWN, balloon->speed);
      break;
    case UP:
    case DOWN:
      next_pos_1 = pos_by_dir(balloon->pos, LEFT, balloon->speed);
      next_pos_2 = pos_by_dir(balloon->pos, RIGHT, balloon->speed);
      break;
  }
  OBJECT next_tile_1 = tile_at(map, map_width, map_height, next_pos_1);
  OBJECT next_tile_2 = tile_at(map, map_width, map_height, next_pos_2);

  return !(balloon->is_valid_tile(next_tile_1) || balloon->is_valid_tile(next_tile_2));
}

void AI_Balloon::choose_new_dir()
{
  std::array<DIRE, 4> dirs{{LEFT, RIGHT, UP, DOWN}};
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle(dirs.begin(), dirs.end(), std::default_random_engine(seed));
  for (const DIRE& _d : dirs)
  {
    XY next_pos = pos_by_dir(balloon->pos, _d, balloon->speed);
    OBJECT next_tile = tile_at(map, map_width, map_height, next_pos);
    if (balloon->is_valid_tile(next_tile))
    {
      dir = _d;
      break;
    }
  }
}
