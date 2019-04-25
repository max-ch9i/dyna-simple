#ifndef AI_BALLOON_H

#define AI_BALLOON_H

#include "Balloon.hpp"

class AI_Balloon
{
  Balloon* balloon;
  DIRE dir;

  void choose_new_dir();
  bool check_if_only_direction();
  public:
    AI_Balloon(Balloon* _b):dir(LEFT)
    {
      assign(_b);
    }
    void move();
    void assign(Balloon* _b);
};

#endif /* end of include guard: AI_BALLOON_H */
