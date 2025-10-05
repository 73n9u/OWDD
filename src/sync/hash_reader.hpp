#ifndef HASH_READER_HPP
#define HASH_READER_HPP

#include <array>
#include <functional>
#include <string>

/**
 * Read hashes from a hash file sequentially
 *
 * @param hashFilePath Path to the hash file to read
 * @param callback Function called for each hash (hash, blockNumber)
 * @return Total number of hashes read
 */
long long readHashFile(
    const std::string &hashFilePath,
    std::function<void(const std::array<unsigned char, 32> &, long long)>
        callback);

/**
 * Read a specific hash from a hash file by block number
 *
 * @param hashFilePath Path to the hash file
 * @param blockNumber Block number to read (0-indexed)
 * @return The 32-byte hash at that position
 */
std::array<unsigned char, 32> readHashAtBlock(const std::string &hashFilePath,
                                              long long blockNumber);

#endif
