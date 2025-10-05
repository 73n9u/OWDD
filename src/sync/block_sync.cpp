#include "detect_diff.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0]
              << " <old_hash_file> <new_source> <blocksize>\n";
    std::cerr << "Example: " << argv[0] << " old.hash /dev/sda1 4096\n";
    return 1;
  }

  const std::string oldHashPath = argv[1];
  const std::string newSourcePath = argv[2];
  size_t blockSize = std::stoul(argv[3]);

  try {
    std::cout << "Comparing hashes...\n";

    long long changedCount =
        detectChangedBlocks(oldHashPath, newSourcePath, blockSize,
                            [](long long blockNum, size_t offset) {
                              std::cout << "Block " << blockNum
                                        << " changed (offset " << offset
                                        << " bytes)\n";
                            });

    std::cout << "\nTotal changed blocks: " << changedCount << "\n";

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
