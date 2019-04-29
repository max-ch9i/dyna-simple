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
  XY dyna;
  XY balloon;

  try {
    OUTCOME outcome;
    ACTION action;
    load_game(file_name, &map_width, &map_height, &outcome);

    std::cout << map_width << " " << map_height << std::endl;
    std::cout << "Outcome: " << outcome << std::endl;
    std::cout << std::endl;

    map = new OBJECT[map_width * map_height];

    while(read_state(&dyna, &balloon, &action, map, map_width, map_height))
    {
      // Paint the map
      for (int y = 0; y < map_height; ++y)
      {
        for (int x = 0; x < map_width; ++x)
        {
          int p = y * map_width + x;

          if (dyna.x == x && dyna.y == y)
            std::cout << "D" << " ";
          else if (balloon.x == x && balloon.y == y)
            std::cout << "B" << " ";
          else
            std::cout << map[p] << " ";

          if (x == map_width - 1)
            std::cout << std::endl;
        }
      }

      std::cout << "Action: " << action << std::endl;
      std::cout << std::endl;
    }
  }
  catch(const _wrong_game_data_read& e)
  {
    std::cout << "Error: " << e.what() << std::endl;
  }


  delete[] map;
  close_game();
}
