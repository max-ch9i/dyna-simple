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

void open_file();
void close_file();
void write_2_byte(int data);
void write_1_byte(int data);
void write_buff(char* buff, int len);

#endif /* end of include guard: WRITER_H */
