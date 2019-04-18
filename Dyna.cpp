#include "Dyna.hpp"

Dyna::Dyna(XY starting_position):Character(starting_position)
{
  draw_colour = COLOUR::WHITE;
};

/**
 * @return {XY} position to put the cracker on
 */
bool Dyna::place_cracker(XY& p)
{
  // Check if a new one is available

  // Check there is no cracker on the tile already
  p = pos;
  return true;
}

void Dyna::slay()
{
  Character::slay();
  draw_colour = COLOUR::BLACK;
}

bool Dyna::is_valid_tile(OBJECT tile)
{
  return tile == _;
}
