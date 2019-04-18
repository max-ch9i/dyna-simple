#ifndef UTILS_H

#define UTILS_H

#include <cairo/cairo-xlib.h>

enum class COLOUR
{
  RED,
  GREEN,
  BLUE,
  GREY,
  BROWN,
  WHITE,
  ORANGE,
  BLACK
};

struct XY
{
  int x;
  int y;
};

enum DIRE
{
  LEFT,
  RIGHT,
  DOWN,
  UP
};

enum OBJECT
{
  _, // Empty
  W, // Wall
  D, // Debris
  B // Boundary
};

#define SLAIN 0x1

#define TIMEOUT 0x1
#define EXPLODING 0x2
#define GONE 0x4

#define MAX(a,b) (a<b)?b:a

#define PI 3.1415926535

#define TILE_WIDTH 100

void win_choose_colour(cairo_t* cr, COLOUR c);

typedef struct win {
    Display *dpy;
    int scr;

    Window win;
    GC gc;

    int width, height;
    KeyCode quit_code;
} win_t;

struct DrawSquareAt
{
  cairo_t* cr;
  DrawSquareAt(cairo_t* c):cr(c){};
  void draw(const COLOUR& colour, const XY& pos) {
    win_choose_colour(cr, colour);
    cairo_rectangle(cr, pos.x * TILE_WIDTH,  pos.y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
    cairo_fill (cr);
  };
};


const int map_width = 5;
const int map_height = 5;
const OBJECT map[map_width * map_height] = {
	  _,   _,   _,   _,   D,
	  _,   W,   _,   W,   _,
	  _,   D,   _,   _,   _,
	  _,   W,   D,   W,   _,
	  _,   _,   _,   _,   _,
};

OBJECT tile_at(const OBJECT* map_mat, int map_width, XY coord);

#endif /* end of include guard: UTILS_H */
