#ifndef DYNA_H

#define DYNA_H
#include "Character.hpp"

class Dyna : public Character
{
  public:
    Dyna(XY starting_position);

    /**
     * @return {XY} position to put the cracker on
     */
    bool place_cracker(XY& p);

  private:
    bool is_valid_tile(OBJECT tile);
};



#endif /* end of include guard: DYNA_H */
