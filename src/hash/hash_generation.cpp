#include "hasher.hpp"
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <unistd.h>

/**
 * Generate hash file from a source file/device
 *
 * @param sourcePath Path to file or device to hash
 * @param blockSize Size of each block in bytes
 * @param outputPath Path to output hash file
 */
void generateHashFile(const char *sourcePath, size_t blockSize,
                      const std::string &outputPath) {

  // Open the disk first
  int fd = open(sourcePath, O_RDONLY);

  if (fd == -1) {
    throw std::runtime_error(std::string("Failed to open source file: ") +
                             sourcePath);
  }

  // Create an output stream for where to write the hashes to
  std::ofstream hashFile(outputPath, std::ios::binary);
  // Throw an errror if the file is open
  if (!hashFile.is_open()) {
    // Close the source file.
    close(fd);
    throw std::runtime_error("Failed to open output file: " + outputPath);
  }

  long long totalBlocks = 0;

  // Iterate over the blocks of the sourcefile and calculate the hash of each of
  // them, writing to the ofstream
  ssize_t bytesRead;
  unsigned char buffer[blockSize];
  while ((bytesRead = read(fd, buffer, blockSize)) > 0) {
    auto hash = calcHash(buffer, bytesRead);
    // write the raw binary of the hash to file
    hashFile.write(reinterpret_cast<const char *>(hash.data()), hash.size());
    // increment the blockCounter
    totalBlocks++;
  }
  if (bytesRead == -1) {
    close(fd);
    throw std::runtime_error(
        std::string("Reading block encountered an error:") + strerror(errno));
  }

  close(fd);
  hashFile.close();
  std::cout << "Done! Generated " << totalBlocks << " hashes.\n";
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <source> <blocksize> <output>\n";
    std::cerr << "Example: " << argv[0] << " /dev/sda1 4096 disk.hash\n";
    return 1;
  }

  const char *source = argv[1];
  size_t blockSize = std::stoul(argv[2]);
  std::string output = argv[3];

  try {
    generateHashFile(source, blockSize, output);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
