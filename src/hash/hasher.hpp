#ifndef HASHER_HPP
#define HASHER_HPP

#include <array>
#include <cstddef>
#include <string>

// Calculate SHA-256 hash and return as 32-byte binary array
std::array<unsigned char, 32> calcHash(const unsigned char *inputStream,
                                       size_t size);

// Convert binary hash to hex string for display/debugging
std::string hashToHex(const std::array<unsigned char, 32> &hash);

#endif
