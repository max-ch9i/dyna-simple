#include "writer.hpp"
#include "game_reader.hpp"
#include <iostream>

static OUTCOME interpret_outcome(int data);
static ACTION interpret_action(int data);
static OBJECT interpret_object(char data);

static int file_end;

void load_game(char* file_name, int* map_width, int* map_height, OUTCOME* outcome)
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
  *outcome = interpret_outcome(a);

  get_file_end(&file_end);
}

bool read_state(XY* dyna, XY* balloon, ACTION* action, OBJECT* map, int map_width, int map_height)
{
  const int len = map_width * map_height;
  char buff[len];

  read_buff(buff, len);
  for (int y = 0; y < map_height; ++y)
  {
    for (int x = 0; x < map_width; ++x)
    {
      int p = y * map_width + x;
      map[p] = interpret_object(buff[p]);

      if (buff[p] & (0x1 << 2))
        (*dyna) = XY{x,y};

      if (buff[p] & (0x1 << 4))
        (*balloon) = XY{x,y};
    }
  }

  read_action(action);

  int file_pos;
  get_file_pos(&file_pos);
  return file_pos < (file_end - 2);
};

void read_action(ACTION* act)
{
  int a;
  read_1_byte(&a);
  (*act) = interpret_action(a);
}

static OBJECT interpret_object(char data)
{
  char tile_data = data & 0x3;
  switch(tile_data)
  {
    case 0x0:
      return _;
    case 0x1:
      return D;
    case 0x2:
      return W;
    default:
      throw _wrong_game_data_read("Invalid tile value");
  }
}

static ACTION interpret_action(int data)
{
  switch(data)
  {
    case 0x1:
      return MOVE_UP;
    case 0x2:
      return MOVE_DOWN;
    case 0x3:
      return MOVE_LEFT;
    case 0x4:
      return MOVE_RIGHT;
    case 0x5:
      return PLANT_CRACKER;
    case 0x6:
      return OTHER;
    default:
      throw _wrong_game_data_read("Invalid action value");
  }
}

static OUTCOME interpret_outcome(int data)
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
