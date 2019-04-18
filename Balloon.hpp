#ifndef BALLOON_H

#define BALLOON_H

#include "Character.hpp"

class Balloon : public Character
{
  public:
    Balloon(XY starting_position);

  private:
    bool is_valid_tile(OBJECT tile);
};

#endif /* end of include guard: BALLOON_H */
