#include "../hash/hasher.hpp"
#include "compressors.hpp"
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <zlib.h>

// Open hash file
// Open dest drive
// Read hash at bNum in file and drive
// Compare
// If same, bNum++
// If diff, compress data

void compareHash(const char *sourcePath, const char *sourceDrive,
                 size_t blockSize, CompressionType compression,
                 const std::string &outputPath) {

  // Open source drive
  int sd = open(sourceDrive, O_RDONLY);

  if (sd == -1) {
    throw std::runtime_error(std::string("Failed to open source drive: ") +
                             sourceDrive);
  }

  // Open output file
  std::ofstream of(outputPath);

  // Open hash file containing the last iteration of hashes of the source
  // disk
  std::ifstream hashFile;
  hashFile.open(sourcePath, std::ios::binary);

  // Determine how many bytes of the source drive are left to be read
  ssize_t bytesRead;

  // Buffer for the bytes of the source of the source drive
  unsigned char diskBuffer[blockSize];

  // Buffer for the current hash of the last iteration of source disk hashes
  char hashBuffer[constants::HASH_SIZE];

  // Iterate over both the file and the drive
  // Condition on while the bytesRead of drive is greater than 0
  while ((bytesRead = read(sd, diskBuffer, blockSize)) > 0) {

    // Read the hash from the source hash file
    hashFile.read(hashBuffer, constants::HASH_SIZE);

    // Define an array that the bytes of the hashFile will be copied into
    std::array<unsigned char, constants::HASH_SIZE> fileHash;
    std::memcpy(fileHash.data(), hashBuffer, constants::HASH_SIZE);

    // Calculate the source disk hash
    auto diskHash = calcHash(diskBuffer, bytesRead);

    // Compare if the last hash calculated in the file has changed with the hash
    // at the same location on the source disk
    if (fileHash != diskHash) {
      std::cout << "fileHash: " << fileHash.data()
                << " is different to diskHash: " << diskHash.data()
                << std::endl;
      // Compress the data at current block on the disk
      unsigned char *compressedData;
      switch (compression) {
      case CompressionType::GZIP:
        compressGZIP(diskBuffer, blockSize, compressedData);
      case CompressionType::LZMA:
        compressLZMA(diskBuffer, blockSize, compressedData);
      case CompressionType::BZIP2:
        compressBZIP2(diskBuffer, blockSize, compressedData);
      }
      std::cout << "Compressed data into output buffer." << "\n";
      // Write compressed buffer to the output file
      of << compressedData << std::endl;

    } else {
      std::cout << "fileHash: " << fileHash.data()
                << " matches  diskHash: " << diskHash.data() << std::endl;
    }
  }
}
int main(int argc, char *argv[]) {}
