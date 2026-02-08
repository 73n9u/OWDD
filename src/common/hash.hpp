#ifndef HASH_HPP
#define HASH_HPP

#include "constants.hpp"
#include <array>
#include <cstdint>

// Class for hash object serialization and deserialization
class Hash {
public:
  std::array<unsigned char, constants::HASH_SIZE> hash;
  uint64_t blockNum;

  // Serialize
};
#endif
