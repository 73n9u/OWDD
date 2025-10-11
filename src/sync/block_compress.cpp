#include "block_compress.hpp"
#include "compressors.hpp"
#include "detect_diff.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

long long detectAndCompressChangedBlocks(
    const std::string &oldHashPath,
    const std::string &newSourcePath,
    size_t blockSize,
    const std::string &outputDir,
    const std::string &compressionType) {

  std::cout << "Detecting and compressing changed blocks...\n";
  std::cout << "Compression type: " << compressionType << "\n";
  std::cout << "Output directory: " << outputDir << "\n\n";

  long long changedCount = detectChangedBlocksWithData(
      oldHashPath, newSourcePath, blockSize,
      [&outputDir, &compressionType](long long blockNum, size_t offset,
                                      const char *blockData, size_t dataSize) {
        // Generate output filename: block_<blocknum>_<offset>.gz/bz2/xz
        std::stringstream ss;
        ss << "block_" << std::setfill('0') << std::setw(10) << blockNum
           << "_" << std::setw(16) << offset;

        std::string baseFilename = ss.str();
        std::string filename;

        // Compress the block data
        if (compressionType == "gzip") {
          filename = baseFilename + ".gz";
          std::string fullPath = outputDir + "/" + filename;
          compressGZIP(blockData, dataSize, fullPath);
          std::cout << "Compressed block " << blockNum << " -> " << filename
                    << " (" << dataSize << " bytes)\n";
        } else if (compressionType == "bzip2") {
          filename = baseFilename + ".bz2";
          std::string fullPath = outputDir + "/" + filename;
          compressBZIP2(blockData, dataSize, fullPath.c_str());
          std::cout << "Compressed block " << blockNum << " -> " << filename
                    << " (" << dataSize << " bytes)\n";
        } else if (compressionType == "lzma" || compressionType == "xz") {
          filename = baseFilename + ".xz";
          std::string fullPath = outputDir + "/" + filename;
          compressLZMA(blockData, dataSize, fullPath);
          std::cout << "Compressed block " << blockNum << " -> " << filename
                    << " (" << dataSize << " bytes)\n";
        } else {
          std::cerr << "Unknown compression type: " << compressionType << "\n";
        }
      });

  std::cout << "\nTotal blocks compressed: " << changedCount << "\n";
  return changedCount;
}
