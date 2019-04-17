#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <memory>

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

using namespace std;

#define PI 3.1415926535

typedef struct win {
    Display *dpy;
    int scr;

    Window win;
    GC gc;

    int width, height;
    KeyCode quit_code;
} win_t;

static void
win_init(win_t *win)
{
    Window root;

    win->width = 400;
    win->height = 400;

    root = DefaultRootWindow(win->dpy);
    win->scr = DefaultScreen(win->dpy);

    win->win = XCreateSimpleWindow(win->dpy, root, 0, 0,
				   win->width, win->height, 0,
				   BlackPixel(win->dpy, win->scr), BlackPixel(win->dpy, win->scr));

    win->quit_code = XKeysymToKeycode(win->dpy, XStringToKeysym("Q"));

    XSelectInput(win->dpy, win->win,
		 KeyPressMask
		 |StructureNotifyMask
		 |ExposureMask);

    XMapWindow(win->dpy, win->win);
}

static void
win_deinit(win_t *win)
{
    XDestroyWindow(win->dpy, win->win);
}



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

    bool check_collision_with_coord(const XY& _p) const
    {
      return _p.x == pos.x && _p.y == pos.y;
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

    bool blast_collides_with(const Character* p) const
    {
      for (const XY& c : blast_cells)
      {
        if (p->check_collision_with_coord(c))
          return true;
      }
      return false;
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
      check_cracker_collisions();
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
    void check_cracker_collisions()
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
};

#define TILE_WIDTH 100

enum class COLOUR
{
  RED,
  GREEN,
  BLUE,
  GREY,
  BROWN,
  WHITE
};

void win_choose_colour(cairo_t* cr, COLOUR c)
{
  switch(c)
  {
    case COLOUR::GREEN:
      cairo_set_source_rgb(cr, 0, 1, 0);
      break;
    case COLOUR::RED:
      cairo_set_source_rgb(cr, 1, 0, 0);
      break;
    case COLOUR::GREY:
      cairo_set_source_rgb(cr, .2, .2, .2);
      break;
    case COLOUR::BLUE:
      cairo_set_source_rgb(cr, 0, 0, 1);
      break;
    case COLOUR::BROWN:
      cairo_set_source_rgb(cr, 1, 0, 1);
      break;
    case COLOUR::WHITE:
      cairo_set_source_rgb(cr, 1, 1, 1);
      break;
  }
}

static void win_draw(win_t *win, Game* game)
{
  cairo_surface_t *surface;
  cairo_t *cr;
  Visual *visual = DefaultVisual(win->dpy, DefaultScreen(win->dpy));

  surface = cairo_xlib_surface_create (win->dpy, win->win, visual, 500, 500);
  cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint (cr);

  // Draw map
  for (int y = 0; y < map_height; ++y)
  {
    for (int x = 0; x < map_width; ++x)
    {
      OBJECT tile = map[y * map_width + x];
      switch(tile)
      {
        case _:
          win_choose_colour(cr, COLOUR::GREEN);
          break;
        case W:
          win_choose_colour(cr, COLOUR::GREY);
          break;
        case D:
          win_choose_colour(cr, COLOUR::BROWN);
          break;
        case B:
          win_choose_colour(cr, COLOUR::BROWN);
          break;
      }
      cairo_rectangle(cr, x * TILE_WIDTH,  y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
      cairo_fill (cr);

    }
  }
}


int main()
{
  win_t win;

  win.dpy = XOpenDisplay(0);

  win_init(&win);

  
  
  Dyna dyna(XY{0,3});
  Balloon balloon(XY{2,0});

  Game game(&dyna);
  game.add_critter(&balloon);

  int tick = 0;
  bool quit = false;
  XEvent xev;
  KeyCode quit_code = XKeysymToKeycode(win.dpy, XStringToKeysym("Q"));
  KeyCode right_code = XKeysymToKeycode(win.dpy, XStringToKeysym("D"));
  KeyCode left_code = XKeysymToKeycode(win.dpy, XStringToKeysym("S"));
  KeyCode up_code = XKeysymToKeycode(win.dpy, XStringToKeysym("W"));
  KeyCode down_code = XKeysymToKeycode(win.dpy, XStringToKeysym("A"));
  KeyCode crack_code = XKeysymToKeycode(win.dpy, XStringToKeysym("F"));
  
  win_draw(&win, &game);
  while (true)
  {
    if (quit)
    {
      break;
    }
    balloon.move(LEFT);

    XNextEvent(win.dpy, &xev);
    switch(xev.type) {
      case KeyPress:
        {
          XKeyEvent *kev = &xev.xkey;

          if (kev->keycode == quit_code) {
            quit = true;
          }
          else if (kev->keycode == right_code) {
            dyna.move(RIGHT);
          }
          else if (kev->keycode == left_code) {
            dyna.move(LEFT);
          }
          else if (kev->keycode == up_code) {
            dyna.move(UP);
          }
          else if (kev->keycode == down_code) {
            dyna.move(DOWN);
          }
          else if (kev->keycode == crack_code) {
            XY cracker_pos;
            bool cracker_available = dyna.place_cracker(cracker_pos);
            if (cracker_available)
            {
              game.add_cracker(cracker_pos);
            }
          }
        }
        break;
      case Expose:
        {
          XExposeEvent *eev = &xev.xexpose;

          if (eev->count == 0)
            ;
        }
        break;
    }

    game.tick_crackers();
    game.check_collisions();

    // dyna.OutPos();
    // balloon.OutPos();
    // dyna.report();
    ++tick;
    win_draw(&win, &game);
  }

  win_deinit(&win);

  XCloseDisplay(win.dpy);
}
