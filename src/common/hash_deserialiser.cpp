#include "hash_deserialiser.h"
#include "exceptions.h"
#include "hash.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <unistd.h>

HashDeserialiser::HashDeserialiser(const std::string inputFile)
    : fdInput{open(inputFile.c_str(), O_RDONLY)} {
  if (fdInput == -1)
    throw FileOpenException(
        inputFile,
        "Hash file to deserialise doesn't exist or was unable to be opened.");
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
    throw FileReadException("Complete binary hash object couldn't be read.");
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
