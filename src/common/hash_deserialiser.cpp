#include "hash_deserialiser.h"
#include "hash.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>

HashDeserialiser::HashDeserialiser(const std::string inputFile)
    : fdInput{open(inputFile.c_str(), O_RDONLY)} {
  if (fdInput == -1)
    throw std::runtime_error("Failed to open output file: " + inputFile);
}
HashDeserialiser::~HashDeserialiser() { close(fdInput); }

bool HashDeserialiser::read(Hash &output) {
  unsigned char inputBuffer[OBJSIZE];
  ssize_t bytesRead = ::read(fdInput, inputBuffer, OBJSIZE);
  if (bytesRead == 0) {
    std::cout << "Reached the end of the file" << std::endl;
    return false;
  }
  if (bytesRead != OBJSIZE) {
    throw std::runtime_error("Failed to read complete binary hash object");
  }

  uint64_t blockNum{0};
  // Read 8 bytes into the blockNum
  for (int i{0}; i < sizeof(uint64_t); i++)
    blockNum = (blockNum << 8) | inputBuffer[i];

  std::array<unsigned char, 32> hashArray;
  std::copy(inputBuffer + sizeof(uint64_t), inputBuffer + OBJSIZE,
            hashArray.begin());

  output = Hash{hashArray, blockNum};
  return true;
}
