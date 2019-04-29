#include <iostream>
#include "game_reader.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cout << "Add a filename." << std::endl;
    return 1;
  }
  char* file_name = argv[1];
  int map_width = 0;
  int map_height = 0;
  OBJECT* map = nullptr;

  try {
    OUTCOME outcome;
    load_game(file_name, map, &map_width, &map_height, &outcome);
    std::cout << map_width << " " << map_height << std::endl;
  }
  catch(const _wrong_game_data_read& e)
  {
    std::cout << "Error: " << e.what() << std::endl;
  }

  close_game();
}
