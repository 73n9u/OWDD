#include "detect_diff.hpp"
#include "../hash/block_reader.hpp"
#include "../hash/hasher.hpp"
#include "hash_reader.hpp"
#include <iostream>

long long detectChangedBlocks(const std::string &oldHashPath,
                              const std::string &newSourcePath,
                              size_t blockSize,
                              std::function<void(long long, size_t)> callback) {
  // Load all old hashes into memory for comparison
  std::vector<std::array<unsigned char, 32>> oldHashes;

  readHashFile(
      oldHashPath,
      [&oldHashes](const std::array<unsigned char, 32> &hash,
                   long long /*blockNum*/) { oldHashes.push_back(hash); });

  long long changedCount = 0;

  // Iterate through new source file blocks and compare hashes
  iterateBlocks(newSourcePath.c_str(), blockSize,
                [&oldHashes, &changedCount, &callback,
                 blockSize](const char *buf, size_t size, long long blockNum) {
                  // Calculate hash of current block
                  auto newHash = calcHash(buf, size);

                  // Check if block number is beyond old hash file (file grew)
                  if (blockNum >= static_cast<long long>(oldHashes.size())) {
                    callback(blockNum, blockNum * blockSize);
                    changedCount++;
                    return;
                  }

                  // Compare with old hash
                  if (newHash != oldHashes[blockNum]) {
                    std::cout << "Old hash: " << hashToHex(oldHashes[blockNum])
                              << std::endl;
                    std::cout << "New hash: " << hashToHex(newHash)
                              << std::endl;

                    callback(blockNum, blockNum * blockSize);
                    changedCount++;
                  }
                });

  // Check if file shrunk (fewer blocks than before)
  // This would mean some blocks at the end were removed
  // We might want to handle this case differently depending on use case

  return changedCount;
}

std::vector<ChangedBlock> getChangedBlocks(const std::string &oldHashPath,
                                           const std::string &newSourcePath,
                                           size_t blockSize) {
  std::vector<ChangedBlock> changed;

  detectChangedBlocks(oldHashPath, newSourcePath, blockSize,
                      [&changed, blockSize](long long blockNum, size_t offset) {
                        changed.push_back({blockNum, offset});
                      });

  return changed;
}
