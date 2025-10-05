#include "hash_reader.hpp"
#include <fstream>
#include <stdexcept>

long long readHashFile(
    const std::string &hashFilePath,
    std::function<void(const std::array<unsigned char, 32> &, long long)>
        callback) {
  std::ifstream hashFile(hashFilePath, std::ios::binary);

  if (!hashFile.is_open()) {
    throw std::runtime_error("Failed to open hash file: " + hashFilePath);
  }

  std::array<unsigned char, 32> hash;
  long long blockNumber = 0;

  while (hashFile.read(reinterpret_cast<char *>(hash.data()), 32)) {
    callback(hash, blockNumber);
    blockNumber++;
  }

  // Check if we stopped due to error or just EOF
  if (hashFile.bad()) {
    throw std::runtime_error("Error reading hash file: " + hashFilePath);
  }

  hashFile.close();
  return blockNumber;
}

std::array<unsigned char, 32> readHashAtBlock(const std::string &hashFilePath,
                                               long long blockNumber) {
  std::ifstream hashFile(hashFilePath, std::ios::binary);

  if (!hashFile.is_open()) {
    throw std::runtime_error("Failed to open hash file: " + hashFilePath);
  }

  // Seek to the position of the hash (each hash is 32 bytes)
  hashFile.seekg(blockNumber * 32, std::ios::beg);

  if (!hashFile.good()) {
    throw std::runtime_error("Failed to seek to block " +
                             std::to_string(blockNumber));
  }

  std::array<unsigned char, 32> hash;
  if (!hashFile.read(reinterpret_cast<char *>(hash.data()), 32)) {
    throw std::runtime_error("Failed to read hash at block " +
                             std::to_string(blockNumber));
  }

  hashFile.close();
  return hash;
}
