#ifndef GAME_READER_H

#define GAME_READER_H

#include "Utils.hpp"
#include "Dyna.hpp"
#include "Balloon.hpp"
#include "Game.hpp"
#include <exception>
#include <string>

class _wrong_game_data_read: public std::exception
{
  std::string err;

  public:
    virtual const char* what() const throw()
    {
      return err.c_str();
    }
    _wrong_game_data_read(std::string _err):err(_err)
    {
    }
};

void load_game(char* file_name, int* map_width, int* map_height, OUTCOME* outcome);
bool read_state(XY* dyna, XY* balloon, ACTION* action, OBJECT* map, int map_width, int map_height);
void read_action(ACTION* act);
void close_game();

#endif /* end of include guard: GAME_READER_H */
