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
void save_action(ACTION dr);
void save_outcome(OUTCOME out);
void end_game();
SAVE_STATE;

#endif /* end of include guard: GAME_WRITER_H */
