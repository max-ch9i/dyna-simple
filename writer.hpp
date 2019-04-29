#ifndef WRITER_H

#define WRITER_H

#include <exception>
#include <fstream>

class _wrong_data: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Wrong data";
  }
};

void open_file(const char* name);
void open_file_read(const char* name);
void close_file();
void write_2_byte(int data);
void write_1_byte(int data);
void write_buff(char* buff, int len);

void read_2_byte(int* data);
void read_1_byte(int* data);
void read_buff(char* buff, int len);
void read_last_2_chars(int* data);

#endif /* end of include guard: WRITER_H */
