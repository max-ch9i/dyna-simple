#include "Game.hpp"

Game::Game(Dyna* d):dyna(d)
{
}

void Game::add_critter(Character* c)
{
  critters.push_back(c);
}

void Game::add_cracker(XY pos)
{
  crackers.push_back(Cracker(pos));
}

void Game::check_collisions()
{
  check_critter_collisions();
  check_cracker_collisions();
  check_wall_collisions();
}

void Game::tick_crackers()
{
  for (auto it = crackers.begin(); it != crackers.end();)
  {
    it->tick();

    if (it->is_gone())
      it = crackers.erase(it);
    else
      ++it;
  }
}

void Game::draw_critters(DrawSquareAt* draw)
{
  for (const Character* c : critters)
  {
    c->draw(draw);
  }
}
void Game::draw_crackers(DrawSquareAt* draw)
{
  for (const Cracker& c : crackers)
  {
    c.draw(draw);
  }
}
void Game::draw_dyna(DrawSquareAt* draw)
{

  dyna->draw(draw);
}

void Game::check_critter_collisions()
{
  for (const Character* c : critters)
  {
    if (dyna->check_collision_with(c))
    {
      dyna->slay();
      break;
    }
  }
}
void Game::check_cracker_collisions()
{
  for (const Cracker& cracker : crackers)
  {
    for (auto it = critters.begin(); it != critters.end();)
    {
      if (cracker.blast_collides_with(*it)) {
        (*it)->slay();
        it = critters.erase(it);
      }
      else
        ++it;
    }
    if (cracker.blast_collides_with(dyna))
    {
      dyna->slay();
    }
  }
}
void Game::check_wall_collisions()
{
  for (const Cracker& cracker : crackers)
  {
    cracker.blast_collide_(incinerate_wall_at);
  }
}
