#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <memory>

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include "Utils.hpp"
#include "Dyna.hpp"
#include "Balloon.hpp"
#include "Game.hpp"

using namespace std;

static void
win_init(win_t *win)
{
    Window root;

    win->width = 500;
    win->height = 500;

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

static void win_draw(win_t *win, Game* game)
{
  cairo_surface_t *surface;
  cairo_t *cr;
  Visual *visual = DefaultVisual(win->dpy, DefaultScreen(win->dpy));

  XClearWindow(win->dpy, win->win);

  surface = cairo_xlib_surface_create (win->dpy, win->win, visual, 500, 500);
  cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

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

  // All critters
  DrawSquareAt draw_at(cr);
  game->draw_critters(&draw_at);
  game->draw_dyna(&draw_at);
  game->draw_crackers(&draw_at);

  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  XFlush(win->dpy);
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
  KeyCode left_code = XKeysymToKeycode(win.dpy, XStringToKeysym("A"));
  KeyCode up_code = XKeysymToKeycode(win.dpy, XStringToKeysym("W"));
  KeyCode down_code = XKeysymToKeycode(win.dpy, XStringToKeysym("S"));
  KeyCode crack_code = XKeysymToKeycode(win.dpy, XStringToKeysym("J"));

  while (true)
  {
    if (quit)
    {
      break;
    }
    // balloon.move(LEFT);

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

          game.tick_crackers();
          game.check_collisions();
          ++tick;
          win_draw(&win, &game);
        }
        break;
      case Expose:
        {
          XExposeEvent *eev = &xev.xexpose;

          if (eev->count == 0)
            win_draw(&win, &game);
        }
        break;
    }
  }
  cout << tick << endl;

  win_deinit(&win);

  XCloseDisplay(win.dpy);
}
