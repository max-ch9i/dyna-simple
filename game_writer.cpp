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
 * <playground>
 * 1 byte action
 * 2 byte outcome <victory|defeat|timeout>
 */

/**
 * <playground>
 *
 * 1 byte cell state
 * ...
 */

/**
 * <cell state>
 *
 * 2 bit cell type
 * 2 bit dyna
 * 1 bit balloon
 * 1 bit cracker pos
 * 1 bit blast pos
 * ...
 */
void start_game(int map_width, int map_height)
{
  using namespace std::chrono;
  unsigned long ms = duration_cast< milliseconds >(
      system_clock::now().time_since_epoch()
      ).count();
  std::string file_name = "rep_" + std::to_string(ms) + ".txt";
  open_file(file_name.c_str());
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

static char cracker_to_char()
{
  return 0x1 << 5;
}

static char blast_to_char()
{
  return 0x1 << 6;
}

void save_state(
    const OBJECT* map,
    int map_width,
    int map_height,
    const Character* dyna,
    const Character* balloon,
    const Game* game)
{
  int length = map_width * map_height;
  char* buff = new char[length];

  // Set the map
  for (int i = 0; i < length; ++i)
  {
    buff[i] = tile_to_char(map[i]);
  }

  // Add the dyna
  buff[map_width * dyna->pos.y + dyna->pos.x] |= dyna_to_char(dyna);

  // Add the balloon
  buff[map_width * balloon->pos.y + balloon->pos.x] |= character_to_char();

  // Add a cracker and blast cells
  for (const Cracker& c : game->crackers )
  {
    buff[map_width * c.pos.y + c.pos.x] |= cracker_to_char();
    for (const XY& b : c.blast_cells)
    {
      buff[map_width * b.y + b.x] |= blast_to_char();
    }
  }

  write_buff(buff, length);
  delete[] buff;
}

void save_action(ACTION dr)
{
  int c;

  switch(dr)
  {
    case MOVE_UP:
      c = 0x1;
      break;
    case MOVE_DOWN:
      c = 0x2;
      break;
    case MOVE_LEFT:
      c = 0x3;
      break;
    case MOVE_RIGHT:
      c = 0x4;
      break;
    case PLANT_CRACKER:
      c = 0x5;
      break;
    case OTHER:
      c = 0x6;
      break;
  }
  write_1_byte(c);
}

void save_outcome(OUTCOME out)
{
  int c = 0x7F00;

  switch(out)
  {
    case VICTORY:
      c |= 0x1;
      break;
    case DEFEAT:
      c |= 0x2;
      break;
    case RESTART:
      c |= 0x3;
      break;
    case PENDING:
      c |= 0x4;
      break;
  }

  write_2_byte(c);
}

void end_game()
{
  close_file();
}
