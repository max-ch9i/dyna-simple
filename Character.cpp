#include "Character.hpp"
#include "Utils.hpp"
#include <iostream>

Character::Character(XY starting_position):pos(starting_position)
{ };

void Character::move(DIRE dir)
{
  if (state & SLAIN)
    return;

  XY next_pos = pos_by_dir(pos, dir, speed);

  OBJECT next_tile = tile_at(map, map_width, map_height, next_pos);

  if (next_tile == B)
    return;

  if (!is_valid_tile(next_tile))
    return;

  prev_pos = pos;
  pos = next_pos;
}

void Character::OutPos() const
{
  std::cout << pos.x << ":" << pos.y << std::endl;
}

bool Character::check_collision_with(const Character* ch)
{
  if (prev_pos.x == ch->pos.x && prev_pos.y == ch->pos.y
      && pos.x == ch->prev_pos.x && pos.y == ch->prev_pos.y)
    return true;

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

bool Character::is_slain() const
{
  return state & SLAIN;
}

void Character::report()
{
  if (state & SLAIN)
    std::cout << "I'm slain" << std::endl;
  else
    std::cout << "I'm healthy." << std::endl;
}

