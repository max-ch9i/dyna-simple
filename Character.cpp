#include "Character.hpp"
#include "Utils.hpp"
#include <iostream>

Character::Character(XY starting_position):pos(starting_position)
{ };

void Character::move(DIRE dir)
{
  if (state & SLAIN)
    return;

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

  // Check map boundaries
  if (next_pos.x >= map_width || next_pos.y >= map_height)
  {
    return;
  }
  else if (next_pos.x < 0 || next_pos.y < 0)
  {
    return;
  }

  OBJECT next_tile = tile_at(map, map_width, next_pos);

  if (!is_valid_tile(next_tile))
    return;

  pos = next_pos;
}

void Character::OutPos() const
{
  std::cout << pos.x << ":" << pos.y << std::endl;
}

bool Character::check_collision_with(const Character* ch)
{
  return ch->pos.x == pos.x && ch->pos.y == pos.y;
}

bool Character::check_collision_with_coord(const XY& _p) const
{
  return _p.x == pos.x && _p.y == pos.y;
}

void Character::draw(DrawSquareAt* context) const
{
  context->draw(draw_colour, pos);
}

void Character::slay()
{
  state |= SLAIN;
}

void Character::report()
{
  if (state & SLAIN)
    std::cout << "I'm slain" << std::endl;
  else
    std::cout << "I'm healthy." << std::endl;
}

