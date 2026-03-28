#include "hash.h"
#include <array>
#include <iomanip>
#include <iostream>
#include <openssl/evp.h>

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

Hash Hash::calcHash(EVP_MD_CTX *mdctx, const unsigned char *inputStream,
                    size_t size, uint64_t blockNum) {
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int hashLength;

  const EVP_MD *md = EVP_sha256();

  EVP_DigestInit_ex(mdctx, md, nullptr);
  EVP_DigestUpdate(mdctx, inputStream, size);
  EVP_DigestFinal_ex(mdctx, hash, &hashLength);

  std::array<unsigned char, 32> result;
  std::copy(hash, hash + hashLength, result.begin());

  return Hash{result, blockNum};
}
