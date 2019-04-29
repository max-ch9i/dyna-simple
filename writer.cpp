#include "writer.hpp"

#define FILE_NAME "test.txt"

static std::fstream _file;

/**
 * `buffer[0] = n; buffer[1] = n >> 8; buffer[2] = n >> 16;`...
  `n = buffer[0]; n |= buffer[1] >> 8; n |= buffer[2] >> 16;`...
*/

_wrong_data wrong_data;

void open_file(const char* name)
{
  _file.open(name, std::fstream::out | std::fstream::binary);
}

void open_file_read(const char* name)
{
  _file.open(name, std::fstream::in | std::fstream::binary);
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

void read_1_byte(int* data)
{
  char buff;

  _file.read(&buff, 1);

  (*data) = buff;
}

void read_buff(char* buff, int len)
{
  _file.read(buff, len);
}

void read_2_byte(int* data)
{
  char buff[2];

  _file.read(buff, 2);

  (*data) = buff[0];
  (*data) |= buff[1] << 8;
}

void read_last_2_chars(int* data)
{
  int current_pos = _file.tellg();

  _file.seekg(-2, _file.end);
  read_2_byte(data);
  _file.seekg(current_pos, _file.beg);
}
