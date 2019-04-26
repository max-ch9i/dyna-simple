#include "writer.hpp"

#define FILE_NAME "test.txt"

static std::fstream _file;

/**
 * `buffer[0] = n; buffer[1] = n >> 8; buffer[2] = n >> 16;`...
  `n = buffer[0]; n |= buffer[1] >> 8; n |= buffer[2] >> 16;`...
*/

_wrong_data wrong_data;

void open_file()
{
  _file.open(FILE_NAME, std::fstream::out | std::fstream::binary);
}

void close_file()
{
  _file.close();
}

void write_1_byte(int data)
{
  if (data < 0 || data > 255)
    throw wrong_data;

  char buff = data;

  _file.write(&buff, 1);
}

void write_buff(char* buff, int len)
{
  _file.write(buff, len);
}

void write_2_byte(int data)
{
  if (data < 0 || data > 65535)
    throw wrong_data;

  char buff[2];
  uint16_t _data = data;

  buff[0] = _data;
  buff[1] = _data >> 8;

  _file.write(buff, 2);
}
