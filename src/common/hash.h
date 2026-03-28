#ifndef HASH_H
#define HASH_H

#include "constants.hpp"
#include <array>
#include <cstdint>

class Hash {
private:
  std::array<unsigned char, constants::HASH_SIZE> m_hash;
  uint64_t m_blockNum;

public:
  Hash(std::array<unsigned char, constants::HASH_SIZE> hash, uint64_t blockNum);

  void printHash();
  void printBlockNum();
  const std::array<unsigned char, constants::HASH_SIZE> &getHash() const {
    return m_hash;
  };
  const uint64_t getBlockNum() const { return m_blockNum; }
};

#endif
