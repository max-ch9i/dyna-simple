#ifndef CHARACTER_H

#define CHARACTER_H

#include "Utils.hpp"

class Character
{
  int speed = 1;
  unsigned char state = 0x0;

  protected:
    XY pos;
    COLOUR draw_colour = COLOUR::WHITE;

  public:
    Character(XY starting_position);
    void move(DIRE dir);
    void OutPos() const;
    bool check_collision_with(const Character* ch);
    bool check_collision_with_coord(const XY& _p) const;
    virtual void draw(DrawSquareAt* context) const;
    void slay();
    void report();

  private:
    virtual bool is_valid_tile(OBJECT tile) = 0;
};

#endif /* end of include guard: CHARACTER_H */
