#ifndef BLOCK_SYNC_HPP
#define BLOCK_SYNC_HPP

#include <string>

/**
 * Detect and compress all changed blocks between old hash and new source
 *
 * @param oldHashPath Path to the old hash file
 * @param newSourcePath Path to the new source file/device
 * @param blockSize Size of blocks used during hashing
 * @param outputDir Directory to write compressed blocks to
 * @param compressionType Type of compression: "gzip", "bzip2", or "lzma"
 * @return Total number of changed blocks detected and compressed
 */
long long detectAndCompressChangedBlocks(
    const std::string &oldHashPath,
    const std::string &newSourcePath,
    size_t blockSize,
    const std::string &outputDir,
    const std::string &compressionType = "gzip");

#endif
