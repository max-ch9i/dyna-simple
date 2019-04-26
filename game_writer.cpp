#include "writer.hpp"
#include "game_writer.hpp"

/**
 * 4 bytes DYNA
 * 2 byte width
 * 2 byte height
 * <playground>
 * 1 byte action
 * <playground>
 * 1 byte action
 * ...
 * 1 byte outcome <victory|defeat|timeout>
 */

/**
 * <playground>
 *
 * 1 byte cell state
 * ...
 */
void start_game(int map_width, int map_height)
{
  open_file();
  write_2_byte(0x5944);
  write_2_byte(0x414E);
  write_2_byte(map_width);
  write_2_byte(map_height);
}

_wrong_game_data wrong_game_data;

static char tile_to_char(OBJECT tile)
{
  switch(tile)
  {
    case _:
      return 0x0;
    case D:
      return 0x1;
    case W:
      return 0x2;
    case B:
      throw wrong_game_data;
      break;
  }
}

static char dyna_to_char(const Character* dyna)
{
  if (dyna->is_slain())
    return 0x3 << 2;
  else
    return 0x1 << 2;
}

static char character_to_char()
{
  return 0x1 << 4;
}

void save_state(
    const OBJECT* map,
    int map_width,
    int map_height,
    const Character* dyna,
    const Character* balloon)
{
  int length = map_width * map_height;
  char* buff = new char[length];

  // Set the map
  for (int i = 0; i < length; ++i)
  {
    buff[i] = tile_to_char(map[i]);
  }

  buff[map_width * dyna->pos.y + dyna->pos.x] |= dyna_to_char(dyna);

  write_buff(buff, length);
  delete[] buff;
}

void end_game()
{
  close_file();
}
