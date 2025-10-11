#ifndef DETECT_DIFF_HPP
#define DETECT_DIFF_HPP

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

/**
 * Structure representing a changed block
 */
struct ChangedBlock {
  long long blockNumber; // Block index in the file
  size_t offset;         // Byte offset in the file
};

/**
 * Compare old hash file with new source file to detect changed blocks
 *
 * @param oldHashPath Path to the old hash file
 * @param newSourcePath Path to the new source file/device
 * @param blockSize Size of blocks used during hashing
 * @param callback Function called for each changed block (blockNumber, offset)
 * @return Total number of changed blocks detected
 */
long long detectChangedBlocks(
    const std::string &oldHashPath, const std::string &newSourcePath,
    size_t blockSize,
    std::function<void(long long, size_t)> callback);

/**
 * Compare old hash file with new source file to detect changed blocks
 * and provide block data for compression or other processing
 *
 * @param oldHashPath Path to the old hash file
 * @param newSourcePath Path to the new source file/device
 * @param blockSize Size of blocks used during hashing
 * @param callback Function called for each changed block (blockNumber, offset, blockData, dataSize)
 * @return Total number of changed blocks detected
 */
long long detectChangedBlocksWithData(
    const std::string &oldHashPath, const std::string &newSourcePath,
    size_t blockSize,
    std::function<void(long long, size_t, const char *, size_t)> callback);

/**
 * Get a vector of all changed block numbers
 *
 * @param oldHashPath Path to the old hash file
 * @param newSourcePath Path to the new source file/device
 * @param blockSize Size of blocks used during hashing
 * @return Vector of changed blocks
 */
std::vector<ChangedBlock> getChangedBlocks(const std::string &oldHashPath,
                                            const std::string &newSourcePath,
                                            size_t blockSize);

#endif
