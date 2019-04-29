#include "writer.hpp"
#include "game_reader.hpp"
#include <iostream>

static OUTCOME read_outcome(int data);

void load_game(char* file_name, OBJECT* map, int* map_width, int* map_height, OUTCOME* outcome)
{
  open_file_read(file_name);
  int a, b;
  read_2_byte(&a);
  read_2_byte(&b);

  if (a != 0x5944 || b != 0x414E)
  {
    throw _wrong_game_data_read("No DYNA");
  }

  read_2_byte(&a);
  read_2_byte(&b);

  if (!(a > 0 && a < 65535) || !(b > 0 && b < 65535))
  {
    throw _wrong_game_data_read("Dimensions are wrong");
  }
  *map_width = a;
  *map_height = b;

  // Get the outcome
  read_last_2_chars(&a);
  *outcome = read_outcome(a);
}

static OUTCOME read_outcome(int data)
{
  if ((data >> 8) != 0x7F)
  {
    throw _wrong_game_data_read("Invalid outcome value");
  }

  int c = data & 0x00FF;

  switch(c)
  {
    case 0x1:
      return VICTORY;
    case 0x2:
      return DEFEAT;
    case 0x3:
      return RESTART;
    case 0x4:
      return PENDING;
    default:
      throw _wrong_game_data_read("Invalid outcome value");
  }
}

void close_game()
{
  close_file();
}
