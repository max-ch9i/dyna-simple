#include "writer.hpp"
#include "game_reader.hpp"

void load_game(char* file_name, OBJECT* map, int* map_width, int* map_height)
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
}

void close_game()
{
  close_file();
}
