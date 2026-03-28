#ifndef HASH_DESERIALISER_H
#define HASH_DESERIALISER_H
#include "hash.h"
#include <cstdint>
#include <string>
class HashDeserialiser {
private:
  int fdInput;
  static constexpr uint8_t OBJSIZE = 40;

public:
  HashDeserialiser(const std::string inputFile);

  ~HashDeserialiser();

  bool read(Hash &output);
};

#endif
