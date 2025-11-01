#include "hasher.hpp"
#include <iomanip>
#include <openssl/evp.h>
#include <sstream>
#include <stdexcept>

/**
 * Calculate SHA-256 hash of a data block
 *
 * @param inputStream Pointer to the input data buffer
 * @param size Size of the input data in bytes
 * @return Fixed-size array containing 32-byte SHA-256 hash
 * @throws std::runtime_error if hash context creation fails
 */
std::array<unsigned char, 32> calcHash(const char *inputStream, size_t size) {
  std::array<unsigned char, 32> result;
  unsigned int hashLength;
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

  if (mdctx == nullptr) {
    throw std::runtime_error(
        "Failed to create EVP_MD_CTX for hash calculation");
  }

  const EVP_MD *md = EVP_sha256();

  EVP_DigestInit_ex(mdctx, md, nullptr);
  EVP_DigestUpdate(mdctx, inputStream, size);
  EVP_DigestFinal_ex(mdctx, result.data(), &hashLength);

  EVP_MD_CTX_free(mdctx);

  return result;
}

/**
 * Convert binary hash to lowercase hexadecimal string
 *
 * @param hash 32-byte SHA-256 hash array
 * @return Lowercase hexadecimal representation (64 characters)
 */
std::string hashToHex(const std::array<unsigned char, 32> &hash) {
  std::stringstream ss;
  for (unsigned char byte : hash) {
    ss << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(byte);
  }
  return ss.str();
}
