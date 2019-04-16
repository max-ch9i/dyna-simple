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

class Character
{
  XY pos;
  int speed = 1;

  public:
    Character(XY starting_position):pos(starting_position)
    {

    };

    void move(DIRE dir)
    {
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

      if (next_tile != _)
      {
        return;
      }

      pos = next_pos;
    }

    void OutPos() const
    {
      cout << pos.x << ":" << pos.y << endl;
    }
};

class Dyna : public Character
{
  public:
    Dyna(XY starting_position):Character(starting_position)
    {

    };

};

int main()
{
  Dyna dyna(XY{0,2});

  int tick = 0;
  while (tick < 10)
  {
    dyna.move(DOWN);
    ++tick;
  }
  dyna.OutPos();
}
