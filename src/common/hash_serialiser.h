#ifndef HASH_SERIALISER_H
#define HASH_SERIALISER_H

#include "hash.h"
#include <string>
#include <vector>

class HashSerialiser {
private:
  int m_fd;
  std::vector<unsigned char> m_writeBuffer;

public:
  HashSerialiser(const std::string &outputPath);
  ~HashSerialiser();
  void write(const Hash &hash);
  void flush();
};

#endif
