#include "../common/constants.hpp"
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
                 size_t blockSize, const std::string &outputPath) {

  // Open destination drive
  int dd = open(sourceDrive, O_RDONLY);

  if (dd == -1) {
    throw std::runtime_error(std::string("Failed to open source drive: ") +
                             sourceDrive);
  }

  // open hash file

  std::ifstream hashFile;
  hashFile.open(sourcePath, std::ios::binary);

  ssize_t bytesRead;
  char diskBuffer[blockSize];
  char fileBuffer[constants::HASH_SIZE];
  // Iterate over both the file and the drive
  // Condition on while the bytesRead of drive is greater than 0

  while ((bytesRead = read(dd, diskBuffer, blockSize)) > 0) {

    // Read the hash from the source hash file
    hashFile.read(fileBuffer, constants::HASH_SIZE);
    std::array<unsigned char, constants::HASH_SIZE> fileHash;
    std::memcpy(fileHash.data(), fileBuffer, constants::HASH_SIZE);

    // calculate the disk hash
    auto diskHash = calcHash(diskBuffer, bytesRead);

    if (fileHash != diskHash) {
      std::cout << "fileHash: " << fileHash.data()
                << " is different to diskHash: " << diskHash.data()
                << std::endl;
      // Compress the data at current block on the disk
      char *compressed_data;

    } else {
      std::cout << "fileHash: " << fileHash.data()
                << " matches  diskHash: " << diskHash.data() << std::endl;
    }
  }
}
int main(int argc, char *argv[]) {}
