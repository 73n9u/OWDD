#ifndef HASH_H
#define HASH_H

#include "constants.h"
#include <array>
#include <cstdint>

#include <openssl/evp.h>
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

  static Hash calcHash(EVP_MD_CTX *mdctx, const unsigned char *inputStream,
                       size_t size, uint64_t blockNum);
};

#endif
