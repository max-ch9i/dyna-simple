#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <memory>
#include <chrono>

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include "Utils.hpp"
#include "Dyna.hpp"
#include "Balloon.hpp"
#include "AI_Balloon.hpp"
#include "Game.hpp"
#include "map_generator.hpp"
#include "game_writer.hpp"

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

const int map_width = 5;
const int map_height = 5;

OBJECT* map = new OBJECT[map_width * map_height];

int main()
{
  win_t win;

  win.dpy = XOpenDisplay(0);

  win_init(&win);

  srand(time(NULL));
  generate_map(map, map_width, map_height);

  Dyna dyna(XY{0,0});
  Balloon balloon = Balloon(choose_balloon_coord(map, map_width, map_height));
  AI_Balloon ai(&balloon);

  Game game(&dyna);
  game.add_critter(&balloon);

  // Open a new file buffer and
  // take the playfield snapshot
  start_game(map_width, map_height);
  save_state(map,
      map_width,
      map_height,
      &dyna,
      &balloon,
      &game);

  int tick = 0;
  bool quit = false;
  XEvent xev;
  KeyCode quit_code = XKeysymToKeycode(win.dpy, XStringToKeysym("T"));
  KeyCode right_code = XKeysymToKeycode(win.dpy, XStringToKeysym("D"));
  KeyCode left_code = XKeysymToKeycode(win.dpy, XStringToKeysym("A"));
  KeyCode up_code = XKeysymToKeycode(win.dpy, XStringToKeysym("W"));
  KeyCode down_code = XKeysymToKeycode(win.dpy, XStringToKeysym("S"));
  KeyCode crack_code = XKeysymToKeycode(win.dpy, XStringToKeysym("J"));
  KeyCode restart_code = XKeysymToKeycode(win.dpy, XStringToKeysym("Q"));

  while (true)
  {
    if (quit)
    {
      break;
    }

    XNextEvent(win.dpy, &xev);
    switch(xev.type) {
      case KeyPress:
        {
          XKeyEvent *kev = &xev.xkey;

          auto start = std::chrono::high_resolution_clock::now();

          // Move balloon
          ai.move();
          // END::Move balloon

          if (kev->keycode == quit_code) {
            quit = true;
          }
          else if (kev->keycode == right_code) {
            dyna.move(RIGHT);
            save_action(MOVE_RIGHT);
          }
          else if (kev->keycode == left_code) {
            dyna.move(LEFT);
            save_action(MOVE_LEFT);
          }
          else if (kev->keycode == up_code) {
            dyna.move(UP);
            save_action(MOVE_UP);
          }
          else if (kev->keycode == down_code) {
            dyna.move(DOWN);
            save_action(MOVE_DOWN);
          }
          else if (kev->keycode == crack_code) {
            save_action(PLANT_CRACKER);
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

          // Check the outcome
          OUTCOME outcome = PENDING;

          if (dyna.is_slain())
            outcome = DEFEAT;
          else if (!game.critters_left())
            outcome = VICTORY;
          else if (tick >= 100)
            outcome = DEFEAT;
          else if (kev->keycode == restart_code)
          {
            outcome = RESTART;
            save_action(OTHER);
          }

          if (outcome != PENDING) {
            save_outcome(outcome);
            end_game();

            generate_map(map, map_width, map_height);

            dyna = Dyna(XY{0,0});
            balloon = Balloon(choose_balloon_coord(map, map_width, map_height));
            ai.assign(&balloon);

            game = Game(&dyna);
            game.add_critter(&balloon);

            start_game(map_width, map_height);
          }

          // Save the state to the buffer
          // file handler
          save_state(map,
              map_width,
              map_height,
              &dyna,
              &balloon,
              &game);

          auto end = std::chrono::high_resolution_clock::now();
          auto dur = end - start;
          auto i_micro = std::chrono::duration_cast<std::chrono::microseconds>(dur);
          cout << "Turn took " << i_micro.count() << "μs" << endl;

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
  cout << "Total number of ticks: " << tick << endl;

  end_game();

  win_deinit(&win);

  XCloseDisplay(win.dpy);
}
