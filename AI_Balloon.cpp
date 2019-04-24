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
  // Check the vicinity
  XY next_pos = pos_by_dir(balloon->pos, dir, balloon->speed);

  OBJECT next_tile = tile_at(map, map_width, map_height, next_pos);

  if (balloon->is_valid_tile(next_tile))
  {
    balloon->move(dir);
  }
  else
  {
    choose_new_dir();
    balloon->move(dir);
  }
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
