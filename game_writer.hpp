#ifndef GAME_WRITER_H

#define GAME_WRITER_H
#include "Utils.hpp"
#include "Dyna.hpp"
#include "Balloon.hpp"
#include "Game.hpp"
#include <exception>

class _wrong_game_data: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Wrong data";
  }
};

void start_game(int map_width, int map_height);
void end_game();
void save_state(
    const OBJECT* map,
    int map_width,
    int map_height,
    const Character* dyna,
    const Character* balloon);

#endif /* end of include guard: GAME_WRITER_H */
