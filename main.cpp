#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <memory>

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

#define TIMEOUT 0x1
#define EXPLODING 0x2
#define GONE 0x4

class Character
{
  int speed = 1;
  unsigned char state = 0x0;

  protected:
    XY pos;

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

    bool check_collision_with(const Character* ch)
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

class Cracker : public Character
{
  static const int timeout = 2;
  static const int blastout = 2;
  unsigned char state = TIMEOUT;
  int timer = 0;
  int blast = 0;
  vector<XY> blast_cells;

  public:
    Cracker(XY starting_position):Character(starting_position)
    {
    };

    void tick()
    {
      handle_timeout();
      handle_explode();
    }

    bool is_gone()
    {
      return state == GONE;
    }

  private:
    void handle_timeout()
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
    void handle_explode()
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
    void update_blast()
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
    bool is_valid_tile(OBJECT tile)
    {
      // It cannot move. Every tile is invalid
      return false;
    }
};

class Dyna : public Character
{
  public:
    Dyna(XY starting_position):Character(starting_position)
    {

    };

    /**
     * @return {XY} position to put the cracker on
     */
    bool place_cracker(XY& p)
    {
      // Check if a new one is available

      // Check there is no cracker on the tile already
      p = pos;
      return true;
    }

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

class Game
{
  vector<Character*> critters;
  vector<Cracker> crackers;
  Dyna* dyna;

  public:
    Game(Dyna* d):dyna(d)
    {
    }
    
    void add_critter(Character* c)
    {
      critters.push_back(c);
    }

    void add_cracker(XY pos)
    {
      crackers.push_back(Cracker(pos));
    }

    void check_collisions()
    {
      check_critter_collisions();
    }

    void tick_crackers()
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

  private:
    void check_critter_collisions()
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
};

int main()
{
  Dyna dyna(XY{0,3});
  Balloon balloon(XY{2,0});

  Game game(&dyna);
  game.add_critter(&balloon);

  int tick = 0;
  while (tick < 10)
  {
    dyna.move(UP);
    balloon.move(LEFT);

    if (tick == 1)
    {
      XY cracker_pos;
      bool cracker_available = dyna.place_cracker(cracker_pos);
      if (cracker_available)
      {
        game.add_cracker(cracker_pos);
      }
    }

    game.tick_crackers();
    game.check_collisions();

    dyna.OutPos();
    balloon.OutPos();
    dyna.report();
    ++tick;
  }
}
