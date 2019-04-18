#include "Cracker.hpp"

Cracker::Cracker(XY starting_position):Character(starting_position)
{
  draw_colour = COLOUR::RED;
};

void Cracker::tick()
{
  handle_timeout();
  handle_explode();
}

bool Cracker::blast_collides_with(const Character* p) const
{
  for (const XY& c : blast_cells)
  {
    if (p->check_collision_with_coord(c))
      return true;
  }
  return false;
}

bool Cracker::is_gone()
{
  return state == GONE;
}

void Cracker::draw(DrawSquareAt* context) const
{
  switch (state)
  {
    case TIMEOUT:
      context->draw(draw_colour, pos);
      break;
    case EXPLODING:
      for (const XY& c : blast_cells)
      {
        context->draw(draw_colour, c);
      }
      break;
  }
}

void Cracker::handle_timeout()
{
  if (state != TIMEOUT)
    return;

  if (timer < timeout)
  {
    ++timer;
    return;
  }
  else
  {
    timer = 0;
    state = EXPLODING;
  }
}
void Cracker::handle_explode()
{
  if (state != EXPLODING)
    return;

  if (blast < blastout)
  {
    ++blast;
    // Update blast
    update_blast();
    return;
  }
  else
  {
    blast = 0;
    state = GONE;
    return;
  }
}
void Cracker::update_blast()
{
  // Update the blast cells
  // For every direction
  blast_cells.clear();
  // The cracker location
  blast_cells.push_back(pos);
  for (int i = 1; i <= blast; ++i)
  {
    // North
    XY pos_next{pos.x,pos.y - i};
    if (pos_next.y >= 0 && _ == tile_at(map, map_width, pos_next))
    {
      blast_cells.push_back(pos_next);
    }
    // South
    pos_next = {pos.x,pos.y + i};
    if (pos_next.y < map_height && _ == tile_at(map, map_width, pos_next))
    {
      blast_cells.push_back(pos_next);
    }
    // West
    pos_next = {pos.x-i,pos.y};
    if (pos_next.x >= 0 && _ == tile_at(map, map_width, pos_next))
    {
      blast_cells.push_back(pos_next);
    }
    // East
    pos_next = {pos.x+i,pos.y};
    if (pos_next.x < map_width && _ == tile_at(map, map_width, pos_next))
    {
      blast_cells.push_back(pos_next);
    }
  }
}
bool Cracker::is_valid_tile(OBJECT tile)
{
  // It cannot move. Every tile is invalid
  return false;
}
