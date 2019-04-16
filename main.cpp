#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>

using namespace std;

struct XY
{
  int x;
  int y;
};

enum OBJECT
{
  _, // Empty
  W, // Wall
  D, // Debris
  B // Boundary
};

static const int map_width = 5;
static const int map_height = 5;
static const OBJECT map[map_width * map_height] = {
	  _,   _,   _,   _,   D,
	  _,   W,   _,   W,   _,
	  _,   D,   _,   _,   _,
	  _,   W,   D,   W,   _,
	  _,   _,   _,   _,   _,
};

#define MAX(a,b) (a<b)?b:a

OBJECT tile_at(const OBJECT* map_mat, int map_width, XY coord)
{
  return map_mat[coord.y * map_width + coord.x];
}


enum DIRE
{
  LEFT,
  RIGHT,
  DOWN,
  UP
};

#define SLAIN 0x1

class Character
{
  XY pos;
  int speed = 1;
  unsigned char state = 0x0;

  public:
    Character(XY starting_position):pos(starting_position)
    {

    };

    void move(DIRE dir)
    {
      if (state & SLAIN)
        return;

      XY next_pos(pos);

      switch (dir)
      {
        case LEFT:
          next_pos.x -= speed;
          break;
        case RIGHT:
          next_pos.x += speed;
          break;
        case DOWN:
          next_pos.y += speed;
          break;
        case UP:
          next_pos.y -= speed;
          break;
      }

      // Check map boundaries
      if (next_pos.x > map_width || next_pos.y > map_height)
      {
        return;
      }
      else if (next_pos.x < 0 || next_pos.y < 0)
      {
        return;
      }

      OBJECT next_tile = tile_at(map, map_width, next_pos);

      if (!is_valid_tile(next_tile))
        return;

      pos = next_pos;
    }

    void OutPos() const
    {
      cout << pos.x << ":" << pos.y << endl;
    }

    bool check_collision_with(Character* ch)
    {
      return ch->pos.x == pos.x && ch->pos.y == pos.y;
    }

    void slay()
    {
      state |= SLAIN;
    }

    void report()
    {
      if (state & SLAIN)
        cout << "I'm slain" << endl;
      else
        cout << "I'm healthy." << endl;
    }

  private:
    virtual bool is_valid_tile(OBJECT tile) = 0;
};

class Dyna : public Character
{
  public:
    Dyna(XY starting_position):Character(starting_position)
    {

    };

  private:
    bool is_valid_tile(OBJECT tile)
    {
      return tile == _;
    }
};

class Balloon : public Character
{
  public:
    Balloon(XY starting_position):Character(starting_position)
    {

    };

  private:
    bool is_valid_tile(OBJECT tile)
    {
      return tile == _;
    }
};

void check_collisions(Character** critters, int critter_num, Dyna* d)
{
  for (int i = 0; i < critter_num; ++i)
  {
    if (d->check_collision_with(critters[i]))
    {
      d->slay();
      break;
    }
  }
}

int main()
{
  Dyna dyna(XY{0,3});
  Balloon balloon(XY{2,0});

  const int critter_num = 1;
  Character* critters[critter_num] = {&balloon};

  int tick = 0;
  while (tick < 10)
  {
    dyna.move(UP);
    balloon.move(LEFT);

    check_collisions(critters, critter_num, &dyna);

    dyna.OutPos();
    balloon.OutPos();
    dyna.report();
    ++tick;
  }
}
