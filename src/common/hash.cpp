#include "hash.h"
#include <array>
#include <iomanip>
#include <iostream>

Hash::Hash(std::array<unsigned char, constants::HASH_SIZE> hash,
           uint64_t blockNum)
    : m_hash{hash}, m_blockNum{blockNum} {}

/**
 * Print from binary hash to lowercase hexadecimal string
 **/
void Hash::printHash() {
  std::stringstream ss;
  for (unsigned char byte : m_hash) {
    ss << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(byte);
  }
  std::cout << "Hash value is: " << ss.str() << std::endl;
}

/**
 * Print uint64_t of hash block number
 **/
void Hash::printBlockNum() {
  std::cout << "Block Number is: " << m_blockNum << std::endl;
}
