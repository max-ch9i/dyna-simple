#ifndef GAME_H

#define GAME_H

#include "Utils.hpp"
#include <vector>
#include "Dyna.hpp"
#include "Cracker.hpp"

class Game
{
  std::vector<Character*> critters;
  std::vector<Cracker> crackers;
  Dyna* dyna;

  public:
    Game(Dyna* d);

    void add_critter(Character* c);

    void add_cracker(XY pos);

    void check_collisions();

    void tick_crackers();

    void draw_critters(DrawSquareAt* draw);

    void draw_crackers(DrawSquareAt* draw);
    void draw_dyna(DrawSquareAt* draw);

  private:
    void check_critter_collisions();
    void check_cracker_collisions();
};

#endif /* end of include guard: GAME_H */