#ifndef HASHER_HPP
#define HASHER_HPP

#include <array>
#include <cstddef>
#include <string>

#include "../common/constants.hpp"
// Calculate SHA-256 hash and return as 32-byte binary array
std::array<unsigned char, constants::HASH_SIZE>
calcHash(const unsigned char *inputStream, size_t size);

// Convert binary hash to hex string for display/debugging
std::string
hashToHex(const std::array<unsigned char, constants::HASH_SIZE> &hash);

#endif
