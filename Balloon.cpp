#include "Balloon.hpp"

Balloon::Balloon(XY starting_position):Character(starting_position)
{
  draw_colour = COLOUR::BLUE;
};

bool Balloon::is_valid_tile(OBJECT tile)
{
  return tile == _;
}
