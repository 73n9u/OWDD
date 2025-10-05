#include "block_reader.hpp"
#include "hasher.hpp"
#include <fstream>
#include <iostream>

/**
 * Generate hash file from a source file/device
 *
 * @param sourcePath Path to file or device to hash
 * @param blockSize Size of each block in bytes
 * @param outputPath Path to output hash file
 */
void generateHashFile(const char *sourcePath, size_t blockSize,
                      const std::string &outputPath) {
  std::ofstream hashFile(outputPath, std::ios::binary);

  if (!hashFile.is_open()) {
    throw std::runtime_error("Failed to open output file: " + outputPath);
  }

  long long totalBlocks = 0;

  iterateBlocks(sourcePath, blockSize,
                [&hashFile, &totalBlocks](const char *buf, size_t size,
                                          long long /*blockNum*/) {
                  auto hash = calcHash(buf, size);
                  hashFile.write(reinterpret_cast<const char *>(hash.data()),
                                 32);
                  totalBlocks++;

                  if (totalBlocks % 1000 == 0) {
                    std::cout << "Processed " << totalBlocks << " blocks...\n";
                  }
                });

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
